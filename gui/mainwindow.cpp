#include "mainwindow.h"
#include <QScrollArea>
#include <QSplitter>
#include <QFrame>
#include <QLineEdit>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>

// ============================================================================
//  Constructor
// ============================================================================

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Matrix Linear Algebra Toolkit — GUI");
    resize(1280, 860);

    m_mainTabs = new QTabWidget(this);
    setCentralWidget(m_mainTabs);

    // Build all tabs
    setupMatrixArithmeticTab();
    setupVectorArithmeticTab();
    setupCoreOpsTab();
    setupNormsTab();
    setupLinearSystemTab();
    setupEigenTab();
    setupLSFittingTab();
    setupBenchmarkTab();
}

// ============================================================================
//  Helper Methods
// ============================================================================

template<typename Func>
void MainWindow::safeCompute(Func&& fn) {
    try {
        fn();
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Computation Error", QString::fromStdString(e.what()));
    } catch (...) {
        QMessageBox::warning(this, "Computation Error", "Unknown error occurred.");
    }
}

QTableWidget* MainWindow::createResultTable(int rows, int cols) {
    auto* tbl = new QTableWidget(rows, cols);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    return tbl;
}

void MainWindow::setResultTableComplex(QTableWidget* tbl, bool complex) {
    int cols = complex ? tbl->columnCount() : tbl->columnCount();
    QStringList headers;
    // This is handled per-case; empty default
    (void)tbl; (void)complex;
}

void MainWindow::showMatrixInTable(QTableWidget* tbl, const Matrix<double>& mat) {
    tbl->setRowCount(mat.rows());
    tbl->setColumnCount(mat.cols());
    QStringList hdr;
    for (int j = 0; j < mat.cols(); ++j) hdr << QString("col%1").arg(j);
    tbl->setHorizontalHeaderLabels(hdr);
    for (int i = 0; i < mat.rows(); ++i)
        for (int j = 0; j < mat.cols(); ++j) {
            auto* item = new QTableWidgetItem(QString::number(mat(i, j), 'g', 8));
            tbl->setItem(i, j, item);
        }
}

void MainWindow::showMatrixInTable(QTableWidget* tbl, const Matrix<Complex>& mat) {
    tbl->setRowCount(mat.rows());
    tbl->setColumnCount(mat.cols() * 2);
    QStringList hdr;
    for (int j = 0; j < mat.cols(); ++j) {
        hdr << QString("col%1.re").arg(j);
        hdr << QString("col%1.im").arg(j);
    }
    tbl->setHorizontalHeaderLabels(hdr);
    for (int i = 0; i < mat.rows(); ++i)
        for (int j = 0; j < mat.cols(); ++j) {
            auto* reItem = new QTableWidgetItem(QString::number(std::real(mat(i, j)), 'g', 8));
            auto* imItem = new QTableWidgetItem(QString::number(std::imag(mat(i, j)), 'g', 8));
            tbl->setItem(i, 2*j, reItem);
            tbl->setItem(i, 2*j+1, imItem);
        }
}

void MainWindow::showVectorInTable(QTableWidget* tbl, const Vector<double>& vec) {
    tbl->setRowCount(vec.rows());
    tbl->setColumnCount(1);
    tbl->setHorizontalHeaderLabels({"value"});
    for (int i = 0; i < vec.rows(); ++i)
        tbl->setItem(i, 0, new QTableWidgetItem(QString::number(vec(i, 0), 'g', 8)));
}

void MainWindow::showVectorInTable(QTableWidget* tbl, const Vector<Complex>& vec) {
    tbl->setRowCount(vec.rows());
    tbl->setColumnCount(2);
    tbl->setHorizontalHeaderLabels({"real", "imag"});
    for (int i = 0; i < vec.rows(); ++i) {
        tbl->setItem(i, 0, new QTableWidgetItem(QString::number(std::real(vec(i, 0)), 'g', 8)));
        tbl->setItem(i, 1, new QTableWidgetItem(QString::number(std::imag(vec(i, 0)), 'g', 8)));
    }
}

void MainWindow::appendResultText(const QString& text) {
    // Used by eigen/ls/bench tabs which have their own QTextEdit*
    // This is a no-op here; those tabs directly access their text edits
}

void MainWindow::appendResultSep() {
    // no-op at this level
}

// ============================================================================
//  Helper: Build a standard "Input → [Compute] → Result" layout for a tab
// ============================================================================
static QLayout* makeComputeLayout(QWidget* inputArea, QPushButton* btn,
                                   QWidget* resultArea) {
    auto* lay = new QVBoxLayout();
    lay->addWidget(inputArea);
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(btn);
    btnRow->addStretch();
    lay->addLayout(btnRow);
    lay->addWidget(resultArea);
    return lay;
}

// ============================================================================
//  SECTION 1: Matrix Arithmetic Tab
// ============================================================================

void MainWindow::setupMatrixArithmeticTab() {
    auto* subTabs = new QTabWidget();

    // ── 1.1 Add ──
    {
        auto* page = new QWidget();
        auto* lay  = new QHBoxLayout(page);
        auto* left = new QVBoxLayout();

        m_addA = new MatrixInputWidget("Matrix A");
        m_addB = new MatrixInputWidget("Matrix B");
        auto* inputs = new QHBoxLayout();
        inputs->addWidget(m_addA);
        inputs->addWidget(m_addB);
        left->addLayout(inputs);

        m_addBtn = new QPushButton("Compute A + B");
        m_addBtn->setMinimumHeight(36);
        m_addBtn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(m_addBtn);

        m_addResult = createResultTable(1, 1);
        m_addResult->setMinimumHeight(200);
        left->addWidget(new QLabel("Result:"));
        left->addWidget(m_addResult);
        lay->addLayout(left);
        subTabs->addTab(page, "1.1 Add");
        connect(m_addBtn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeMatrixAdd(); }); });
    }

    // ── 1.2 Sub ──
    {
        auto* page = new QWidget();
        auto* lay  = new QHBoxLayout(page);
        auto* left = new QVBoxLayout();

        m_subA = new MatrixInputWidget("Matrix A");
        m_subB = new MatrixInputWidget("Matrix B");
        auto* inputs = new QHBoxLayout();
        inputs->addWidget(m_subA);
        inputs->addWidget(m_subB);
        left->addLayout(inputs);

        m_subBtn = new QPushButton("Compute A - B");
        m_subBtn->setMinimumHeight(36);
        m_subBtn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(m_subBtn);

        m_subResult = createResultTable(1, 1);
        m_subResult->setMinimumHeight(200);
        left->addWidget(new QLabel("Result:"));
        left->addWidget(m_subResult);
        lay->addLayout(left);
        subTabs->addTab(page, "1.2 Sub");
        connect(m_subBtn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeMatrixSub(); }); });
    }

    // ── 1.3 Mul ──
    {
        auto* page = new QWidget();
        auto* lay  = new QHBoxLayout(page);
        auto* left = new QVBoxLayout();

        m_mulA = new MatrixInputWidget("Matrix A");
        m_mulB = new MatrixInputWidget("Matrix B");
        auto* inputs = new QHBoxLayout();
        inputs->addWidget(m_mulA);
        inputs->addWidget(m_mulB);
        left->addLayout(inputs);

        m_mulBtn = new QPushButton("Compute A × B");
        m_mulBtn->setMinimumHeight(36);
        m_mulBtn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(m_mulBtn);

        m_mulResult = createResultTable(1, 1);
        m_mulResult->setMinimumHeight(200);
        left->addWidget(new QLabel("Result:"));
        left->addWidget(m_mulResult);
        lay->addLayout(left);
        subTabs->addTab(page, "1.3 Mul");
        connect(m_mulBtn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeMatrixMul(); }); });
    }

    // ── 1.4 Scalar ──
    {
        auto* page = new QWidget();
        auto* lay  = new QHBoxLayout(page);
        auto* left = new QVBoxLayout();

        m_scalarA = new MatrixInputWidget("Matrix A");
        left->addWidget(m_scalarA);

        auto* scalarRow = new QHBoxLayout();
        scalarRow->addWidget(new QLabel("Scalar (real):"));
        m_scalarSpinReal = new QDoubleSpinBox();
        m_scalarSpinReal->setRange(-1e9, 1e9);
        m_scalarSpinReal->setValue(2.0);
        m_scalarSpinReal->setDecimals(6);
        scalarRow->addWidget(m_scalarSpinReal);

        scalarRow->addWidget(new QLabel("Imag (if complex):"));
        m_scalarImagEdit = new QLineEdit("0");
        m_scalarImagEdit->setMaximumWidth(100);
        m_scalarComplexRow = m_scalarImagEdit;
        scalarRow->addWidget(m_scalarImagEdit);
        scalarRow->addStretch();
        left->addLayout(scalarRow);

        m_scalarBtn = new QPushButton("Compute Scalar × / ÷");
        m_scalarBtn->setMinimumHeight(36);
        m_scalarBtn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(m_scalarBtn);

        left->addWidget(new QLabel("A × scalar:"));
        m_scalarMulResult = createResultTable(1, 1);
        left->addWidget(m_scalarMulResult);

        left->addWidget(new QLabel("A ÷ scalar:"));
        m_scalarDivResult = createResultTable(1, 1);
        left->addWidget(m_scalarDivResult);

        lay->addLayout(left);
        subTabs->addTab(page, "1.4 Scalar");
        connect(m_scalarBtn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeMatrixScalarOp(); }); });
    }

    // ── 1.5 Mat × Vec ──
    {
        auto* page = new QWidget();
        auto* lay  = new QHBoxLayout(page);
        auto* left = new QVBoxLayout();

        m_mvA = new MatrixInputWidget("Matrix A");
        m_mvV = new VectorInputWidget("Vector v");
        auto* inputs = new QHBoxLayout();
        inputs->addWidget(m_mvA);
        inputs->addWidget(m_mvV);
        left->addLayout(inputs);

        m_mvBtn = new QPushButton("Compute A × v");
        m_mvBtn->setMinimumHeight(36);
        m_mvBtn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(m_mvBtn);

        m_mvResult = createResultTable(1, 1);
        m_mvResult->setMinimumHeight(200);
        left->addWidget(new QLabel("Result:"));
        left->addWidget(m_mvResult);
        lay->addLayout(left);
        subTabs->addTab(page, "1.5 Mat×Vec");
        connect(m_mvBtn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeMatrixVectorMul(); }); });
    }

    m_mainTabs->addTab(subTabs, "① Matrix Arithmetic");
}

// ── Section 1 compute implementations ──

void MainWindow::computeMatrixAdd() {
    if (m_addA->isComplex() || m_addB->isComplex()) {
        auto A = m_addA->getComplexMatrix();
        auto B = m_addB->getComplexMatrix();
        auto R = A + B;
        showMatrixInTable(m_addResult, R);
        QMessageBox::information(this, "Success", "Addition completed.");
    } else {
        auto A = m_addA->getRealMatrix();
        auto B = m_addB->getRealMatrix();
        auto R = A + B;
        showMatrixInTable(m_addResult, R);
        QMessageBox::information(this, "Success", "Addition completed.");
    }
}

void MainWindow::computeMatrixSub() {
    if (m_subA->isComplex() || m_subB->isComplex()) {
        auto A = m_subA->getComplexMatrix();
        auto B = m_subB->getComplexMatrix();
        showMatrixInTable(m_subResult, A - B);
    } else {
        auto A = m_subA->getRealMatrix();
        auto B = m_subB->getRealMatrix();
        showMatrixInTable(m_subResult, A - B);
    }
}

void MainWindow::computeMatrixMul() {
    if (m_mulA->isComplex() || m_mulB->isComplex()) {
        auto A = m_mulA->getComplexMatrix();
        auto B = m_mulB->getComplexMatrix();
        showMatrixInTable(m_mulResult, A * B);
    } else {
        auto A = m_mulA->getRealMatrix();
        auto B = m_mulB->getRealMatrix();
        showMatrixInTable(m_mulResult, A * B);
    }
}

void MainWindow::computeMatrixScalarOp() {
    if (m_scalarA->isComplex()) {
        auto A = m_scalarA->getComplexMatrix();
        double re = m_scalarSpinReal->value();
        double im = m_scalarImagEdit->text().toDouble();
        Complex s(re, im);
        showMatrixInTable(m_scalarMulResult, A * s);
        try {
            showMatrixInTable(m_scalarDivResult, A / s);
        } catch (const std::exception& e) {
            m_scalarDivResult->clear();
            m_scalarDivResult->setRowCount(1); m_scalarDivResult->setColumnCount(1);
            m_scalarDivResult->setItem(0, 0, new QTableWidgetItem(QString("Error: %1").arg(e.what())));
        }
    } else {
        auto A = m_scalarA->getRealMatrix();
        double s = m_scalarSpinReal->value();
        showMatrixInTable(m_scalarMulResult, A * s);
        try {
            showMatrixInTable(m_scalarDivResult, A / s);
        } catch (const std::exception& e) {
            m_scalarDivResult->clear();
            m_scalarDivResult->setRowCount(1); m_scalarDivResult->setColumnCount(1);
            m_scalarDivResult->setItem(0, 0, new QTableWidgetItem(QString("Error: %1").arg(e.what())));
        }
    }
}

void MainWindow::computeMatrixVectorMul() {
    if (m_mvA->isComplex() || m_mvV->isComplex()) {
        auto A = m_mvA->getComplexMatrix();
        auto v = m_mvV->getComplexVector();
        showVectorInTable(m_mvResult, A * v);
    } else {
        auto A = m_mvA->getRealMatrix();
        auto v = m_mvV->getRealVector();
        showVectorInTable(m_mvResult, A * v);
    }
}

// ============================================================================
//  SECTION 2: Vector Arithmetic Tab
// ============================================================================

void MainWindow::setupVectorArithmeticTab() {
    auto* subTabs = new QTabWidget();

    // ── 2.1 Vector Add ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        auto* row = new QHBoxLayout();
        m_vAdd1 = new VectorInputWidget("Vector v1");
        m_vAdd2 = new VectorInputWidget("Vector v2");
        row->addWidget(m_vAdd1);
        row->addWidget(m_vAdd2);
        left->addLayout(row);
        auto* btn = new QPushButton("Compute v1 + v2");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Result:"));
        m_vAddResult = createResultTable(1, 1);
        left->addWidget(m_vAddResult);
        subTabs->addTab(page, "2.1 Add");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorAdd(); }); });
    }

    // ── 2.2 Vector Sub ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        auto* row = new QHBoxLayout();
        m_vSub1 = new VectorInputWidget("Vector v1");
        m_vSub2 = new VectorInputWidget("Vector v2");
        row->addWidget(m_vSub1);
        row->addWidget(m_vSub2);
        left->addLayout(row);
        auto* btn = new QPushButton("Compute v1 - v2");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Result:"));
        m_vSubResult = createResultTable(1, 1);
        left->addWidget(m_vSubResult);
        subTabs->addTab(page, "2.2 Sub");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorSub(); }); });
    }

    // ── 2.3 Dot Product ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        auto* row = new QHBoxLayout();
        m_vDot1 = new VectorInputWidget("Vector v1");
        m_vDot2 = new VectorInputWidget("Vector v2");
        row->addWidget(m_vDot1);
        row->addWidget(m_vDot2);
        left->addLayout(row);
        auto* btn = new QPushButton("Compute v1 · v2");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        m_vDotResult = new QLabel("Result: —");
        m_vDotResult->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
        left->addWidget(m_vDotResult);
        left->addStretch();
        subTabs->addTab(page, "2.3 Dot");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorDot(); }); });
    }

    // ── 2.4 Cross Product ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        auto* row = new QHBoxLayout();
        m_vCross1 = new VectorInputWidget("Vector v1 (3D)");
        m_vCross1->setDimension(3);
        m_vCross2 = new VectorInputWidget("Vector v2 (3D)");
        m_vCross2->setDimension(3);
        row->addWidget(m_vCross1);
        row->addWidget(m_vCross2);
        left->addLayout(row);
        auto* btn = new QPushButton("Compute v1 × v2");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Result:"));
        m_vCrossResult = createResultTable(1, 1);
        left->addWidget(m_vCrossResult);
        subTabs->addTab(page, "2.4 Cross");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorCross(); }); });
    }

    // ── 2.5 Vector Scalar Mul ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_vScalarV = new VectorInputWidget("Vector v");
        left->addWidget(m_vScalarV);
        auto* srow = new QHBoxLayout();
        srow->addWidget(new QLabel("Scalar:"));
        m_vScalarSpin = new QDoubleSpinBox();
        m_vScalarSpin->setRange(-1e9, 1e9);
        m_vScalarSpin->setValue(2.0);
        m_vScalarSpin->setDecimals(6);
        srow->addWidget(m_vScalarSpin);
        srow->addStretch();
        left->addLayout(srow);
        auto* btn = new QPushButton("Compute v × scalar");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Result:"));
        m_vScalarResult = createResultTable(1, 1);
        left->addWidget(m_vScalarResult);
        subTabs->addTab(page, "2.5 Scalar");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorScalarMul(); }); });
    }

    // ── 2.6 Vector Norms ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_vNormsVec = new VectorInputWidget("Vector v");
        left->addWidget(m_vNormsVec);
        auto* prow = new QHBoxLayout();
        prow->addWidget(new QLabel("p (for p-norm):"));
        m_vNormsP = new QDoubleSpinBox();
        m_vNormsP->setRange(1.0, 100.0);
        m_vNormsP->setValue(3.0);
        prow->addWidget(m_vNormsP);
        prow->addStretch();
        left->addLayout(prow);
        auto* btn = new QPushButton("Compute All Norms");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Results:"));
        m_vNormsResult = new QTextEdit();
        m_vNormsResult->setReadOnly(true);
        m_vNormsResult->setMaximumHeight(200);
        m_vNormsResult->setStyleSheet("font-size: 14px;");
        left->addWidget(m_vNormsResult);
        subTabs->addTab(page, "2.6 Norms");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeVectorNorms(); }); });
    }

    m_mainTabs->addTab(subTabs, "② Vector Arithmetic");
}

void MainWindow::computeVectorAdd() {
    if (m_vAdd1->isComplex() || m_vAdd2->isComplex()) {
        showVectorInTable(m_vAddResult, m_vAdd1->getComplexVector() + m_vAdd2->getComplexVector());
    } else {
        showVectorInTable(m_vAddResult, m_vAdd1->getRealVector() + m_vAdd2->getRealVector());
    }
}

void MainWindow::computeVectorSub() {
    if (m_vSub1->isComplex() || m_vSub2->isComplex()) {
        showVectorInTable(m_vSubResult, m_vSub1->getComplexVector() - m_vSub2->getComplexVector());
    } else {
        showVectorInTable(m_vSubResult, m_vSub1->getRealVector() - m_vSub2->getRealVector());
    }
}

void MainWindow::computeVectorDot() {
    QString result;
    if (m_vDot1->isComplex() || m_vDot2->isComplex()) {
        auto d = m_vDot1->getComplexVector().dot(m_vDot2->getComplexVector());
        result = QString("v1 · v2 = %1 + %2 i")
            .arg(std::real(d), 0, 'g', 8).arg(std::imag(d), 0, 'g', 8);
    } else {
        double d = m_vDot1->getRealVector().dot(m_vDot2->getRealVector());
        result = QString("v1 · v2 = %1").arg(d, 0, 'g', 8);
    }
    m_vDotResult->setText(result);
}

void MainWindow::computeVectorCross() {
    if (m_vCross1->isComplex() || m_vCross2->isComplex()) {
        showVectorInTable(m_vCrossResult, m_vCross1->getComplexVector().cross(m_vCross2->getComplexVector()));
    } else {
        showVectorInTable(m_vCrossResult, m_vCross1->getRealVector().cross(m_vCross2->getRealVector()));
    }
}

void MainWindow::computeVectorScalarMul() {
    double s = m_vScalarSpin->value();
    if (m_vScalarV->isComplex()) {
        Complex cs(s, 0);
        showVectorInTable(m_vScalarResult, m_vScalarV->getComplexVector() * cs);
    } else {
        showVectorInTable(m_vScalarResult, m_vScalarV->getRealVector() * s);
    }
}

void MainWindow::computeVectorNorms() {
    double p = m_vNormsP->value();
    QString result;
    if (m_vNormsVec->isComplex()) {
        auto v = m_vNormsVec->getComplexVector();
        result += QString("L1   = %1\n").arg(v.norm_l1(), 0, 'g', 8);
        result += QString("L2   = %1\n").arg(v.norm_l2(), 0, 'g', 8);
        result += QString("Linf = %1\n").arg(v.norm_inf(), 0, 'g', 8);
        result += QString("L%1   = %2").arg(p, 0, 'f', 1).arg(v.norm_p(p), 0, 'g', 8);
    } else {
        auto v = m_vNormsVec->getRealVector();
        result += QString("L1   = %1\n").arg(v.norm_l1(), 0, 'g', 8);
        result += QString("L2   = %1\n").arg(v.norm_l2(), 0, 'g', 8);
        result += QString("Linf = %1\n").arg(v.norm_inf(), 0, 'g', 8);
        result += QString("L%1   = %2").arg(p, 0, 'f', 1).arg(v.norm_p(p), 0, 'g', 8);
    }
    m_vNormsResult->setPlainText(result);
}

// ============================================================================
//  SECTION 3: Core Matrix Operations
// ============================================================================

void MainWindow::setupCoreOpsTab() {
    auto* subTabs = new QTabWidget();

    // ── 3.1 Determinant ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_detA = new MatrixInputWidget("Square Matrix A");
        left->addWidget(m_detA);
        auto* btn = new QPushButton("Compute det(A)");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        m_detResult = new QLabel("det(A) = —");
        m_detResult->setStyleSheet("font-size: 18px; font-weight: bold; padding: 15px;");
        left->addWidget(m_detResult);
        left->addStretch();
        subTabs->addTab(page, "3.1 Det");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeDeterminant(); }); });
    }

    // ── 3.2 Adjugate + Inverse ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_adjA = new MatrixInputWidget("Square Matrix A");
        left->addWidget(m_adjA);
        auto* btn = new QPushButton("Compute adj(A) and A⁻¹");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        m_adjDetLabel = new QLabel("");
        m_adjDetLabel->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px;");
        left->addWidget(m_adjDetLabel);
        left->addWidget(new QLabel("Adjugate:"));
        m_adjResult = createResultTable(1, 1);
        left->addWidget(m_adjResult);
        m_adjInvLabel = new QLabel("");
        m_adjInvLabel->setStyleSheet("font-size: 14px; padding: 5px;");
        left->addWidget(m_adjInvLabel);
        left->addWidget(new QLabel("Inverse:"));
        m_invResult = createResultTable(1, 1);
        left->addWidget(m_invResult);
        subTabs->addTab(page, "3.2 Adj+Inv");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeAdjugateInverse(); }); });
    }

    // ── 3.3 Transpose ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_transA = new MatrixInputWidget("Matrix A");
        left->addWidget(m_transA);
        auto* btn = new QPushButton("Compute Aᵀ");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        left->addWidget(new QLabel("Result:"));
        m_transResult = createResultTable(1, 1);
        left->addWidget(m_transResult);
        subTabs->addTab(page, "3.3 Trans");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeTranspose(); }); });
    }

    // ── 3.4 Rank + RREF ──
    {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        m_rrefA = new MatrixInputWidget("Matrix A");
        left->addWidget(m_rrefA);
        auto* btn = new QPushButton("Compute Rank & RREF");
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        m_rankResult = new QLabel("Rank = —");
        m_rankResult->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");
        left->addWidget(m_rankResult);
        left->addWidget(new QLabel("RREF:"));
        m_rrefResult = createResultTable(1, 1);
        left->addWidget(m_rrefResult);
        subTabs->addTab(page, "3.4 Rank+RREF");
        connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeRankRREF(); }); });
    }

    m_mainTabs->addTab(subTabs, "③ Core Operations");
}

void MainWindow::computeDeterminant() {
    if (m_detA->isComplex()) {
        auto A = m_detA->getComplexMatrix();
        auto d = A.determinant();
        m_detResult->setText(QString("det(A) = %1 + %2 i")
            .arg(std::real(d), 0, 'g', 8).arg(std::imag(d), 0, 'g', 8));
    } else {
        auto A = m_detA->getRealMatrix();
        m_detResult->setText(QString("det(A) = %1").arg(A.determinant(), 0, 'g', 8));
    }
}

void MainWindow::computeAdjugateInverse() {
    const double EPS = 1e-10;
    if (m_adjA->isComplex()) {
        auto A = m_adjA->getComplexMatrix();
        auto det = A.determinant();
        m_adjDetLabel->setText(QString("det(A) = %1 + %2 i")
            .arg(std::real(det), 0, 'g', 6).arg(std::imag(det), 0, 'g', 6));
        showMatrixInTable(m_adjResult, A.adjugate());
        if (std::abs(det) > EPS) {
            m_adjInvLabel->setText("");
            showMatrixInTable(m_invResult, A.inverse());
        } else {
            m_adjInvLabel->setText("⚠ Singular matrix — no inverse!");
            m_invResult->clear();
            m_invResult->setRowCount(0); m_invResult->setColumnCount(0);
        }
    } else {
        auto A = m_adjA->getRealMatrix();
        auto det = A.determinant();
        m_adjDetLabel->setText(QString("det(A) = %1").arg(det, 0, 'g', 8));
        showMatrixInTable(m_adjResult, A.adjugate());
        if (std::fabs(det) > EPS) {
            m_adjInvLabel->setText("");
            showMatrixInTable(m_invResult, A.inverse());
        } else {
            m_adjInvLabel->setText("⚠ Singular matrix — no inverse!");
            m_invResult->clear();
            m_invResult->setRowCount(0); m_invResult->setColumnCount(0);
        }
    }
}

void MainWindow::computeTranspose() {
    if (m_transA->isComplex()) {
        showMatrixInTable(m_transResult, m_transA->getComplexMatrix().transpose());
    } else {
        showMatrixInTable(m_transResult, m_transA->getRealMatrix().transpose());
    }
}

void MainWindow::computeRankRREF() {
    if (m_rrefA->isComplex()) {
        auto A = m_rrefA->getComplexMatrix();
        m_rankResult->setText(QString("Rank = %1").arg(A.rank()));
        showMatrixInTable(m_rrefResult, A.rref());
    } else {
        auto A = m_rrefA->getRealMatrix();
        m_rankResult->setText(QString("Rank = %1").arg(A.rank()));
        showMatrixInTable(m_rrefResult, A.rref());
    }
}

// ============================================================================
//  SECTION 4: Matrix Norms & Condition Number
// ============================================================================

void MainWindow::setupNormsTab() {
    auto* subTabs = new QTabWidget();

    auto makeNormPage = [&](const QString& title, MatrixInputWidget*& mat, QLabel*& label,
                             QPushButton*& btn, const QString& btnText,
                             std::function<void()> computeFn) {
        auto* page = new QWidget();
        auto* left = new QVBoxLayout(page);
        mat = new MatrixInputWidget("Matrix A");
        left->addWidget(mat);
        btn = new QPushButton(btnText);
        btn->setMinimumHeight(36); btn->setStyleSheet("font-weight: bold; font-size: 14px;");
        left->addWidget(btn);
        label = new QLabel("Result: —");
        label->setStyleSheet("font-size: 18px; font-weight: bold; padding: 15px;");
        label->setWordWrap(true);
        left->addWidget(label);
        left->addStretch();
        subTabs->addTab(page, title);
        connect(btn, &QPushButton::clicked, this, [this, computeFn](){ safeCompute(computeFn); });
    };

    QPushButton *btnF, *btnR, *btnC, *btnS, *btnCond;
    makeNormPage("4.1 Frob",  m_frobA, m_frobResult, btnF,  "Compute ||A||_F",    [this](){ computeFrobeniusNorm(); });
    makeNormPage("4.2 Row",   m_rowA,  m_rowResult,  btnR,  "Compute ||A||_inf",  [this](){ computeRowNorm(); });
    makeNormPage("4.3 Col",   m_colA,  m_colResult,  btnC,  "Compute ||A||_1",    [this](){ computeColNorm(); });
    makeNormPage("4.4 Spec",  m_specA, m_specResult, btnS,  "Compute ||A||_2",    [this](){ computeSpectralNorm(); });
    makeNormPage("4.5 Cond",  m_condA, m_condResult, btnCond,"Compute cond(A)",     [this](){ computeConditionNumber(); });

    m_mainTabs->addTab(subTabs, "④ Norms & Cond");
}

void MainWindow::computeFrobeniusNorm() {
    if (m_frobA->isComplex()) {
        double n = m_frobA->getComplexMatrix().norm_frobenius();
        m_frobResult->setText(QString("||A||_F = %1").arg(n, 0, 'g', 8));
    } else {
        double n = m_frobA->getRealMatrix().norm_frobenius();
        m_frobResult->setText(QString("||A||_F = %1").arg(n, 0, 'g', 8));
    }
}

void MainWindow::computeRowNorm() {
    if (m_rowA->isComplex()) {
        m_rowResult->setText(QString("||A||_∞ = %1").arg(m_rowA->getComplexMatrix().norm_row(), 0, 'g', 8));
    } else {
        m_rowResult->setText(QString("||A||_∞ = %1").arg(m_rowA->getRealMatrix().norm_row(), 0, 'g', 8));
    }
}

void MainWindow::computeColNorm() {
    if (m_colA->isComplex()) {
        m_colResult->setText(QString("||A||_1 = %1").arg(m_colA->getComplexMatrix().norm_col(), 0, 'g', 8));
    } else {
        m_colResult->setText(QString("||A||_1 = %1").arg(m_colA->getRealMatrix().norm_col(), 0, 'g', 8));
    }
}

void MainWindow::computeSpectralNorm() {
    try {
        if (m_specA->isComplex()) {
            m_specResult->setText(QString("||A||_2 = %1").arg(m_specA->getComplexMatrix().norm_spectral(), 0, 'g', 8));
        } else {
            m_specResult->setText(QString("||A||_2 = %1").arg(m_specA->getRealMatrix().norm_spectral(), 0, 'g', 8));
        }
    } catch (const std::exception& e) {
        m_specResult->setText(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::computeConditionNumber() {
    if (m_condA->isComplex()) {
        double c = m_condA->getComplexMatrix().conditionNumber();
        if (c == HUGE_VAL) m_condResult->setText("cond(A) = ∞ (singular)");
        else m_condResult->setText(QString("cond(A) = %1").arg(c, 0, 'g', 8));
    } else {
        double c = m_condA->getRealMatrix().conditionNumber();
        if (c == HUGE_VAL) m_condResult->setText("cond(A) = ∞ (singular or non-square)");
        else m_condResult->setText(QString("cond(A) = %1").arg(c, 0, 'g', 8));
    }
}

// ============================================================================
//  SECTION 5: Linear System Solver
// ============================================================================

void MainWindow::setupLinearSystemTab() {
    auto* page = new QWidget();
    auto* lay  = new QVBoxLayout(page);

    auto* row = new QHBoxLayout();
    m_lsA = new MatrixInputWidget("Coefficient Matrix A");
    m_lsB = new VectorInputWidget("Right-hand Side b");
    row->addWidget(m_lsA);
    row->addWidget(m_lsB);
    lay->addLayout(row);

    auto* btn = new QPushButton("Solve A x = b");
    btn->setMinimumHeight(40); btn->setStyleSheet("font-weight: bold; font-size: 15px;");
    lay->addWidget(btn);

    m_lsStatus = new QLabel("");
    m_lsStatus->setStyleSheet("font-size: 14px; font-weight: bold; padding: 5px;");
    lay->addWidget(m_lsStatus);

    lay->addWidget(new QLabel("Solution x:"));
    m_lsResult = createResultTable(1, 1);
    m_lsResult->setMinimumHeight(150);
    lay->addWidget(m_lsResult);

    m_mainTabs->addTab(page, "⑤ Linear System");
    connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeLinearSystem(); }); });
}

void MainWindow::computeLinearSystem() {
    if (m_lsA->isComplex() || m_lsB->isComplex()) {
        auto A = m_lsA->getComplexMatrix();
        auto b = m_lsB->getComplexVector();
        auto x = A.solveLinear(b);
        if (x.rows() == 0) {
            m_lsStatus->setText("⚠ No solution or inconsistent system!");
            m_lsResult->clear(); m_lsResult->setRowCount(0); m_lsResult->setColumnCount(0);
        } else {
            m_lsStatus->setText("✓ Solved successfully.");
            showVectorInTable(m_lsResult, x);
        }
    } else {
        auto A = m_lsA->getRealMatrix();
        auto b = m_lsB->getRealVector();
        auto x = A.solveLinear(b);
        if (x.rows() == 0) {
            m_lsStatus->setText("⚠ No solution or inconsistent system!");
            m_lsResult->clear(); m_lsResult->setRowCount(0); m_lsResult->setColumnCount(0);
        } else {
            m_lsStatus->setText("✓ Solved successfully.");
            showVectorInTable(m_lsResult, x);
        }
    }
}

// ============================================================================
//  SECTION 6: Eigenvalues & Eigenvectors
// ============================================================================

void MainWindow::setupEigenTab() {
    auto* page = new QWidget();
    auto* lay  = new QVBoxLayout(page);

    m_eigenA = new MatrixInputWidget("Square Matrix A");
    m_eigenA->setDimensions(3, 3);
    lay->addWidget(m_eigenA);

    auto* btn = new QPushButton("Compute Eigenvalues & Eigenvectors");
    btn->setMinimumHeight(40); btn->setStyleSheet("font-weight: bold; font-size: 15px;");
    lay->addWidget(btn);

    lay->addWidget(new QLabel("Results:"));
    m_eigenResult = new QTextEdit();
    m_eigenResult->setReadOnly(true);
    m_eigenResult->setStyleSheet("font-family: Consolas, monospace; font-size: 13px;");
    lay->addWidget(m_eigenResult);

    m_mainTabs->addTab(page, "⑥ Eigenvalues");
    connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeEigen(); }); });
}

void MainWindow::computeEigen() {
    const double EPS = 1e-10;

    auto processEigen = [&](auto& A) {
        using T = typename std::decay<decltype(A(0,0))>::type;
        QString out;
        out += "Matrix A:\n";

        // Format matrix as string
        std::ostringstream oss;
        for (int i = 0; i < A.rows(); ++i) {
            for (int j = 0; j < A.cols(); ++j) {
                oss << std::setw(12) << A(i, j);
            }
            oss << "\n";
        }
        out += QString::fromStdString(oss.str());

        auto eig = A.eigenvalues();
        bool diag = A.isDiagonalizable();

        out += "\n══════ Eigenvalues & Eigenvectors ══════\n\n";

        for (size_t i = 0; i < eig.size(); ++i) {
            Complex lam = eig[i];
            // Skip duplicates
            bool dup = false;
            for (size_t j = 0; j < i; ++j)
                if (std::abs(eig[j] - lam) < EPS) { dup = true; break; }
            if (dup) continue;

            auto vec = A.eigenVector(lam);
            int cnt = 0;
            for (auto x : eig) if (std::abs(x - lam) < EPS) cnt++;

            int n = A.rows();
            Matrix<Complex> Ac = A.template cast<Complex>();
            Matrix<Complex> L(n, n);
            for (int k = 0; k < n; ++k) L(k, k) = lam;
            int g = n - (Ac - L).rank();

            out += QString("λ = %1 + %2 i\n")
                .arg(std::real(lam), 0, 'g', 6).arg(std::imag(lam), 0, 'g', 6);
            out += QString("  Algebraic mult:  %1\n").arg(cnt);
            out += QString("  Geometric mult:  %1\n").arg(g);
            out += "  Eigenvector:\n";
            for (int r = 0; r < vec.rows(); ++r) {
                out += QString("    %1 + %2 i\n")
                    .arg(std::real(vec(r, 0)), 0, 'g', 6)
                    .arg(std::imag(vec(r, 0)), 0, 'g', 6);
            }
            out += "\n";
        }
        out += QString("Diagonalizable?  %1\n").arg(diag ? "YES ✓" : "NO ✗");
        m_eigenResult->setPlainText(out);
    };

    if (m_eigenA->isComplex()) {
        auto A = m_eigenA->getComplexMatrix();
        processEigen(A);
    } else {
        auto A = m_eigenA->getRealMatrix();
        processEigen(A);
    }
}

// ============================================================================
//  SECTION 7: Least Squares Polynomial Fitting
// ============================================================================

void MainWindow::setupLSFittingTab() {
    auto* page = new QWidget();
    auto* lay  = new QVBoxLayout(page);

    // Data points table
    auto* ptsGroup = new QGroupBox("Data Points (x, y)");
    auto* ptsLay   = new QVBoxLayout(ptsGroup);
    auto* ptsCtrl  = new QHBoxLayout();
    ptsCtrl->addWidget(new QLabel("Number of points:"));
    auto* ptsSpin = new QSpinBox();
    ptsSpin->setRange(2, 1000);
    ptsSpin->setValue(5);
    ptsCtrl->addWidget(ptsSpin);
    auto* addRowBtn = new QPushButton("Resize Table");
    ptsCtrl->addWidget(addRowBtn);
    auto* loadPtsBtn = new QPushButton("Load from File...");
    ptsCtrl->addWidget(loadPtsBtn);
    ptsCtrl->addStretch();
    ptsLay->addLayout(ptsCtrl);

    m_lsPointsTable = new QTableWidget(5, 2);
    m_lsPointsTable->setHorizontalHeaderLabels({"x", "y"});
    m_lsPointsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ptsLay->addWidget(m_lsPointsTable);
    lay->addWidget(ptsGroup);

    // Initialize with sample data
    double sample[5][2] = {{0,1}, {1,3}, {2,7}, {3,13}, {4,21}};
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 2; ++j)
            m_lsPointsTable->setItem(i, j, new QTableWidgetItem(QString::number(sample[i][j])));

    connect(ptsSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int n) {
        m_lsPointsTable->setRowCount(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < 2; ++j)
                if (!m_lsPointsTable->item(i, j))
                    m_lsPointsTable->setItem(i, j, new QTableWidgetItem("0"));
    });

    connect(addRowBtn, &QPushButton::clicked, this, [this, ptsSpin]() {
        m_lsPointsTable->setRowCount(ptsSpin->value());
        for (int i = 0; i < ptsSpin->value(); ++i)
            for (int j = 0; j < 2; ++j)
                if (!m_lsPointsTable->item(i, j))
                    m_lsPointsTable->setItem(i, j, new QTableWidgetItem("0"));
    });

    connect(loadPtsBtn, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, "Open Points File",
            "../test_data", "Text Files (*.txt);;All Files (*)");
        if (path.isEmpty()) return;
        try {
            std::ifstream f(path.toStdString());
            std::vector<std::pair<double,double>> pts;
            double x, y;
            while (f >> x >> y) pts.push_back({x, y});
            m_lsPointsTable->setRowCount((int)pts.size());
            for (size_t i = 0; i < pts.size(); ++i) {
                m_lsPointsTable->setItem((int)i, 0, new QTableWidgetItem(QString::number(pts[i].first, 'g', 8)));
                m_lsPointsTable->setItem((int)i, 1, new QTableWidgetItem(QString::number(pts[i].second, 'g', 8)));
            }
        } catch (...) {
            QMessageBox::warning(this, "Error", "Failed to load points file.");
        }
    });

    // Degree and compute
    auto* ctrlRow = new QHBoxLayout();
    ctrlRow->addWidget(new QLabel("Polynomial Degree:"));
    m_lsDegree = new QSpinBox();
    m_lsDegree->setRange(0, 50);
    m_lsDegree->setValue(2);
    ctrlRow->addWidget(m_lsDegree);
    ctrlRow->addStretch();
    auto* btn = new QPushButton("Fit Polynomial");
    btn->setMinimumHeight(40); btn->setStyleSheet("font-weight: bold; font-size: 15px;");
    ctrlRow->addWidget(btn);
    lay->addLayout(ctrlRow);

    m_lsFitResult = new QTextEdit();
    m_lsFitResult->setReadOnly(true);
    m_lsFitResult->setStyleSheet("font-family: Consolas, monospace; font-size: 14px;");
    m_lsFitResult->setMinimumHeight(200);
    lay->addWidget(m_lsFitResult);

    m_mainTabs->addTab(page, "⑦ LS Fitting");
    connect(btn, &QPushButton::clicked, this, [this](){ safeCompute([this](){ computeLeastSquares(); }); });
}

void MainWindow::computeLeastSquares() {
    int n = m_lsPointsTable->rowCount();
    int degree = m_lsDegree->value();

    if (degree >= n) {
        QMessageBox::warning(this, "Warning",
            QString("Degree (%1) >= number of points (%2) — overfitting!").arg(degree).arg(n));
    }

    Vector<double> x_vals(n), y_vals(n);
    for (int i = 0; i < n; ++i) {
        x_vals(i, 0) = m_lsPointsTable->item(i, 0) ? m_lsPointsTable->item(i, 0)->text().toDouble() : 0.0;
        y_vals(i, 0) = m_lsPointsTable->item(i, 1) ? m_lsPointsTable->item(i, 1)->text().toDouble() : 0.0;
    }

    int m = degree + 1;
    Matrix<double> A(n, m);
    for (int i = 0; i < n; ++i) {
        double x = x_vals(i, 0);
        for (int j = 0; j < m; ++j)
            A(i, j) = std::pow(x, j);
    }

    Vector<double> coeffs = A.solveLinear(y_vals);
    if (coeffs.rows() == 0) {
        m_lsFitResult->setPlainText("⚠ Solve failed! Matrix may be rank-deficient.");
        return;
    }

    double sum_sq = 0;
    for (int i = 0; i < n; ++i) {
        double x = x_vals(i, 0), y_pred = 0;
        for (int j = 0; j < m; ++j)
            y_pred += coeffs(j, 0) * std::pow(x, j);
        sum_sq += std::pow(y_vals(i, 0) - y_pred, 2);
    }
    double rmse = std::sqrt(sum_sq / n);

    QString result;
    result += QString("Fitted polynomial (degree %1):\n\n  y = ").arg(degree);
    bool first = true;
    for (int i = 0; i < m; ++i) {
        double c = coeffs(i, 0);
        if (std::fabs(c) < 1e-10) continue;
        if (!first) result += (c > 0 ? " + " : " - ");
        else if (c < 0) result += "-";
        first = false;
        if (i == 0) result += QString::number(std::fabs(c), 'g', 6);
        else if (i == 1) result += QString("%1 x").arg(std::fabs(c), 0, 'g', 6);
        else result += QString("%1 x^%2").arg(std::fabs(c), 0, 'g', 6).arg(i);
    }
    result += "\n\nCoefficients (c₀ + c₁x + c₂x² + ...):\n";
    for (int i = 0; i < m; ++i)
        result += QString("  c%1 = %2\n").arg(i).arg(coeffs(i, 0), 0, 'g', 8);
    result += QString("\nRMSE = %1").arg(rmse, 0, 'g', 6);
    m_lsFitResult->setPlainText(result);
}

// ============================================================================
//  SECTION 8: Matrix Multiplication Performance Benchmark
// ============================================================================

void MainWindow::setupBenchmarkTab() {
    auto* page = new QWidget();
    auto* lay  = new QVBoxLayout(page);

    auto* info = new QLabel(
        "Benchmarks three matrix multiplication strategies:\n"
        "  • Naive triple-loop\n"
        "  • Blocked (64×64 cache-friendly)\n"
        "  • Multi-threaded blocked\n\n"
        "Tests sizes: 256×256, 512×512, 1024×1024\n"
        "⚠ Large sizes may take a while (especially 1024)!"
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 13px; padding: 10px;");
    lay->addWidget(info);

    m_benchBtn = new QPushButton("▶ Run Benchmark");
    m_benchBtn->setMinimumHeight(45);
    m_benchBtn->setStyleSheet("font-weight: bold; font-size: 16px; background-color: #4CAF50; color: white;");
    lay->addWidget(m_benchBtn);

    m_benchProgress = new QProgressBar();
    m_benchProgress->setRange(0, 100);
    m_benchProgress->setValue(0);
    lay->addWidget(m_benchProgress);

    m_benchResult = new QTextEdit();
    m_benchResult->setReadOnly(true);
    m_benchResult->setStyleSheet("font-family: Consolas, monospace; font-size: 13px;");
    lay->addWidget(m_benchResult);

    m_mainTabs->addTab(page, "⑧ Benchmark");

    connect(m_benchBtn, &QPushButton::clicked, this, [this]() {
        m_benchBtn->setEnabled(false);
        m_benchBtn->setText("⏳ Running...");
        QApplication::processEvents();
        safeCompute([this](){ computeBenchmark(); });
        m_benchBtn->setEnabled(true);
        m_benchBtn->setText("▶ Run Benchmark");
    });
}

void MainWindow::computeBenchmark() {
    const int BLOCK_SIZE = 64;
    const int MAX_THREADS = (int)std::thread::hardware_concurrency();
    if (MAX_THREADS <= 0) {
        QMessageBox::warning(this, "Error", "Cannot determine hardware concurrency!");
        return;
    }

    struct BenchMatrix {
        int n;
        std::vector<std::vector<double>> data;
        BenchMatrix(int size) : n(size), data(size, std::vector<double>(size, 0.0)) {}
        double& operator()(int i, int j) { return data[i][j]; }
        const double& operator()(int i, int j) const { return data[i][j]; }
        int size() const { return n; }
        void randomInit() {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    data[i][j] = (std::rand() % 100) / 10.0;
        }
    };

    auto naiveMul = [](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                for (int k = 0; k < n; ++k)
                    C(i, j) += A(i, k) * B(k, j);
        return C;
    };

    auto blockMul = [&](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        for (int i = 0; i < n; i += BLOCK_SIZE)
            for (int j = 0; j < n; j += BLOCK_SIZE)
                for (int k = 0; k < n; k += BLOCK_SIZE)
                    for (int ii = i; ii < std::min(i + BLOCK_SIZE, n); ++ii)
                        for (int jj = j; jj < std::min(j + BLOCK_SIZE, n); ++jj)
                            for (int kk = k; kk < std::min(k + BLOCK_SIZE, n); ++kk)
                                C(ii, jj) += A(ii, kk) * B(kk, jj);
        return C;
    };

    auto threadedMul = [&](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        std::vector<std::thread> threads;
        auto worker = [&](int sr, int er) {
            for (int i = sr; i < er; ++i)
                for (int j = 0; j < n; j += BLOCK_SIZE)
                    for (int k = 0; k < n; k += BLOCK_SIZE)
                        for (int jj = j; jj < std::min(j + BLOCK_SIZE, n); ++jj)
                            for (int kk = k; kk < std::min(k + BLOCK_SIZE, n); ++kk)
                                C(i, jj) += A(i, kk) * B(kk, jj);
        };
        int rpt = n / MAX_THREADS;
        for (int t = 0; t < MAX_THREADS; ++t) {
            int sr = t * rpt, er = (t == MAX_THREADS - 1) ? n : sr + rpt;
            threads.emplace_back(worker, sr, er);
        }
        for (auto& th : threads) th.join();
        return C;
    };

    std::srand((unsigned)std::time(0));
    QString result;
    result += QString("Hardware threads: %1\n").arg(MAX_THREADS);
    result += QString("Block size: %1\n\n").arg(BLOCK_SIZE);

    std::vector<int> sizes = {256, 512, 1024};
    int totalStages = (int)sizes.size();
    int stage = 0;

    using Clock = std::chrono::high_resolution_clock;

    for (int sz : sizes) {
        result += QString("══════ Size: %1 × %1 ══════\n").arg(sz);
        m_benchResult->setPlainText(result);
        m_benchProgress->setValue(stage * 100 / totalStages);
        QApplication::processEvents();

        BenchMatrix A(sz), B(sz);
        A.randomInit(); B.randomInit();

        // Naive
        auto t0 = Clock::now();
        naiveMul(A, B);
        auto t1 = Clock::now();
        double t_naive = std::chrono::duration<double>(t1 - t0).count();
        result += QString("  Naive:          %1 s\n").arg(t_naive, 0, 'f', 4);
        m_benchResult->setPlainText(result);
        QApplication::processEvents();

        // Block
        t0 = Clock::now();
        blockMul(A, B);
        t1 = Clock::now();
        double t_block = std::chrono::duration<double>(t1 - t0).count();
        result += QString("  Block:          %1 s  (×%2 speedup)\n")
            .arg(t_block, 0, 'f', 4).arg(t_naive / t_block, 0, 'f', 2);
        m_benchResult->setPlainText(result);
        QApplication::processEvents();

        // Threaded
        t0 = Clock::now();
        threadedMul(A, B);
        t1 = Clock::now();
        double t_thread = std::chrono::duration<double>(t1 - t0).count();
        result += QString("  Threaded+Block: %1 s  (×%2 speedup)\n\n")
            .arg(t_thread, 0, 'f', 4).arg(t_naive / t_thread, 0, 'f', 2);
        m_benchResult->setPlainText(result);
        QApplication::processEvents();

        stage++;
    }

    result += "═══ Benchmark Complete ═══\n";
    m_benchResult->setPlainText(result);
    m_benchProgress->setValue(100);
}
