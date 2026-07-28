#include "matrix_widget.h"
#include <QGridLayout>
#include <QLineEdit>
#include <sstream>

// ============================================================================
// MatrixInputWidget
// ============================================================================

MatrixInputWidget::MatrixInputWidget(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    buildUI(title);
}

void MatrixInputWidget::buildUI(const QString& title) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto* group = new QGroupBox(title);
    auto* glay  = new QVBoxLayout(group);

    // --- Top controls row ---
    auto* ctrlRow = new QHBoxLayout();
    ctrlRow->addWidget(new QLabel("Rows:"));
    m_rowsSpin = new QSpinBox();
    m_rowsSpin->setRange(1, 50);
    m_rowsSpin->setValue(2);
    ctrlRow->addWidget(m_rowsSpin);

    ctrlRow->addWidget(new QLabel("Cols:"));
    m_colsSpin = new QSpinBox();
    m_colsSpin->setRange(1, 50);
    m_colsSpin->setValue(2);
    ctrlRow->addWidget(m_colsSpin);

    ctrlRow->addWidget(new QLabel("Type:"));
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("Real");
    m_typeCombo->addItem("Complex");
    ctrlRow->addWidget(m_typeCombo);

    m_loadBtn = new QPushButton("Load File...");
    ctrlRow->addWidget(m_loadBtn);
    ctrlRow->addStretch();
    glay->addLayout(ctrlRow);

    // --- Table ---
    m_table = new QTableWidget();
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    glay->addWidget(m_table);

    mainLayout->addWidget(group);

    // Connections
    connect(m_rowsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MatrixInputWidget::onDimensionsChanged);
    connect(m_colsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MatrixInputWidget::onDimensionsChanged);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MatrixInputWidget::onTypeChanged);
    connect(m_loadBtn, &QPushButton::clicked,
            this, &MatrixInputWidget::loadFromFile);

    syncTableDimensions();
}

void MatrixInputWidget::setDimensions(int rows, int cols) {
    m_updating = true;
    m_rowsSpin->setValue(rows);
    m_colsSpin->setValue(cols);
    m_updating = false;
    syncTableDimensions();
}

int MatrixInputWidget::rows() const { return m_rowsSpin->value(); }
int MatrixInputWidget::cols() const { return m_colsSpin->value(); }
bool MatrixInputWidget::isComplex() const { return m_typeCombo->currentIndex() == 1; }

void MatrixInputWidget::onDimensionsChanged() {
    if (!m_updating) {
        syncTableDimensions();
        emit matrixChanged();
    }
}

void MatrixInputWidget::onTypeChanged(int) {
    syncTableDimensions();
    emit matrixChanged();
}

void MatrixInputWidget::syncTableDimensions() {
    int r = rows();
    int c = cols();
    int displayCols = isComplex() ? c * 2 : c;
    m_table->setRowCount(r);
    m_table->setColumnCount(displayCols);

    QStringList headers;
    if (isComplex()) {
        for (int j = 0; j < c; ++j) {
            headers << QString("col%1.re").arg(j);
            headers << QString("col%1.im").arg(j);
        }
    } else {
        for (int j = 0; j < c; ++j)
            headers << QString("col%1").arg(j);
    }
    m_table->setHorizontalHeaderLabels(headers);

    // Initialize empty cells
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < displayCols; ++j)
            if (!m_table->item(i, j))
                m_table->setItem(i, j, new QTableWidgetItem("0"));
}

Matrix<double> MatrixInputWidget::getRealMatrix() const {
    int r = rows(), c = cols();
    Matrix<double> mat(r, c);
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            auto* item = m_table->item(i, j);
            mat(i, j) = item ? item->text().toDouble() : 0.0;
        }
    return mat;
}

Matrix<Complex> MatrixInputWidget::getComplexMatrix() const {
    int r = rows(), c = cols();
    Matrix<Complex> mat(r, c);
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            auto* reItem = m_table->item(i, 2*j);
            auto* imItem = m_table->item(i, 2*j + 1);
            double re = reItem ? reItem->text().toDouble() : 0.0;
            double im = imItem ? imItem->text().toDouble() : 0.0;
            mat(i, j) = Complex(re, im);
        }
    return mat;
}

void MatrixInputWidget::setMatrix(const Matrix<double>& mat) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(0);
    setDimensions(mat.rows(), mat.cols());
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < mat.rows(); ++i)
        for (int j = 0; j < mat.cols(); ++j)
            m_table->item(i, j)->setText(QString::number(mat(i, j), 'g', 6));
}

void MatrixInputWidget::setMatrix(const Matrix<Complex>& mat) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(1);
    setDimensions(mat.rows(), mat.cols());
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < mat.rows(); ++i)
        for (int j = 0; j < mat.cols(); ++j) {
            m_table->item(i, 2*j)->setText(QString::number(std::real(mat(i, j)), 'g', 6));
            m_table->item(i, 2*j+1)->setText(QString::number(std::imag(mat(i, j)), 'g', 6));
        }
}

void MatrixInputWidget::setVector(const Vector<double>& vec) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(0);
    setDimensions(vec.rows(), 1);
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < vec.rows(); ++i)
        m_table->item(i, 0)->setText(QString::number(vec(i, 0), 'g', 6));
}

void MatrixInputWidget::setVector(const Vector<Complex>& vec) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(1);
    setDimensions(vec.rows(), 1);
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < vec.rows(); ++i) {
        m_table->item(i, 0)->setText(QString::number(std::real(vec(i, 0)), 'g', 6));
        m_table->item(i, 1)->setText(QString::number(std::imag(vec(i, 0)), 'g', 6));
    }
}

void MatrixInputWidget::resizeTable(int rows, int cols) {
    setDimensions(rows, cols);
}

void MatrixInputWidget::loadFromFile() {
    QString path = QFileDialog::getOpenFileName(this, "Open Matrix File",
        "../test_data", "Text Files (*.txt);;All Files (*)");
    if (path.isEmpty()) return;

    std::string fname = path.toStdString();
    try {
        if (isComplexFile(fname)) {
            std::ifstream f(fname);
            Matrix<Complex> mat;
            if (loadComplexMatrix(f, mat)) {
                setMatrix(mat);
            } else {
                QMessageBox::warning(this, "Error", "Failed to load complex matrix from file.");
            }
        } else {
            std::ifstream f(fname);
            Matrix<double> mat;
            if (loadRealMatrix(f, mat)) {
                setMatrix(mat);
            } else {
                QMessageBox::warning(this, "Error", "Failed to load real matrix from file.");
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Exception: %1").arg(e.what()));
    }
}

// ============================================================================
// VectorInputWidget
// ============================================================================

VectorInputWidget::VectorInputWidget(const QString& title, QWidget* parent)
    : QWidget(parent)
{
    buildUI(title);
}

void VectorInputWidget::buildUI(const QString& title) {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto* group = new QGroupBox(title);
    auto* glay  = new QVBoxLayout(group);

    auto* ctrlRow = new QHBoxLayout();
    ctrlRow->addWidget(new QLabel("Dimension:"));
    m_dimSpin = new QSpinBox();
    m_dimSpin->setRange(1, 100);
    m_dimSpin->setValue(3);
    ctrlRow->addWidget(m_dimSpin);

    ctrlRow->addWidget(new QLabel("Type:"));
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("Real");
    m_typeCombo->addItem("Complex");
    ctrlRow->addWidget(m_typeCombo);

    m_loadBtn = new QPushButton("Load File...");
    ctrlRow->addWidget(m_loadBtn);
    ctrlRow->addStretch();
    glay->addLayout(ctrlRow);

    m_table = new QTableWidget();
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    glay->addWidget(m_table);

    mainLayout->addWidget(group);

    connect(m_dimSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &VectorInputWidget::onDimChanged);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &VectorInputWidget::onTypeChanged);
    connect(m_loadBtn, &QPushButton::clicked,
            this, &VectorInputWidget::loadFromFile);

    syncTableDimensions();
}

void VectorInputWidget::setDimension(int dim) {
    m_updating = true;
    m_dimSpin->setValue(dim);
    m_updating = false;
    syncTableDimensions();
}

int VectorInputWidget::dimension() const { return m_dimSpin->value(); }
bool VectorInputWidget::isComplex() const { return m_typeCombo->currentIndex() == 1; }

void VectorInputWidget::onDimChanged() {
    if (!m_updating) {
        syncTableDimensions();
        emit vectorChanged();
    }
}

void VectorInputWidget::onTypeChanged(int) {
    syncTableDimensions();
    emit vectorChanged();
}

void VectorInputWidget::syncTableDimensions() {
    int d = dimension();
    int displayCols = isComplex() ? 2 : 1;
    m_table->setRowCount(d);
    m_table->setColumnCount(displayCols);

    QStringList headers;
    if (isComplex()) {
        headers << "real" << "imag";
    } else {
        headers << "value";
    }
    m_table->setHorizontalHeaderLabels(headers);
    m_table->setVerticalHeaderLabels({});

    for (int i = 0; i < d; ++i)
        for (int j = 0; j < displayCols; ++j)
            if (!m_table->item(i, j))
                m_table->setItem(i, j, new QTableWidgetItem("0"));
}

Vector<double> VectorInputWidget::getRealVector() const {
    int d = dimension();
    Vector<double> v(d);
    for (int i = 0; i < d; ++i) {
        auto* item = m_table->item(i, 0);
        v(i, 0) = item ? item->text().toDouble() : 0.0;
    }
    return v;
}

Vector<Complex> VectorInputWidget::getComplexVector() const {
    int d = dimension();
    Vector<Complex> v(d);
    for (int i = 0; i < d; ++i) {
        auto* reItem = m_table->item(i, 0);
        auto* imItem = m_table->item(i, 1);
        double re = reItem ? reItem->text().toDouble() : 0.0;
        double im = imItem ? imItem->text().toDouble() : 0.0;
        v(i, 0) = Complex(re, im);
    }
    return v;
}

void VectorInputWidget::setVector(const Vector<double>& vec) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(0);
    setDimension(vec.rows());
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < vec.rows(); ++i)
        m_table->item(i, 0)->setText(QString::number(vec(i, 0), 'g', 6));
}

void VectorInputWidget::setVector(const Vector<Complex>& vec) {
    m_updating = true;
    m_typeCombo->setCurrentIndex(1);
    setDimension(vec.rows());
    m_updating = false;
    syncTableDimensions();
    for (int i = 0; i < vec.rows(); ++i) {
        m_table->item(i, 0)->setText(QString::number(std::real(vec(i, 0)), 'g', 6));
        m_table->item(i, 1)->setText(QString::number(std::imag(vec(i, 0)), 'g', 6));
    }
}

void VectorInputWidget::loadFromFile() {
    QString path = QFileDialog::getOpenFileName(this, "Open Vector File",
        "../test_data", "Text Files (*.txt);;All Files (*)");
    if (path.isEmpty()) return;

    std::string fname = path.toStdString();
    try {
        std::ifstream f(fname);
        if (isComplexFile(fname)) {
            auto v = readComplexVec(f);
            setVector(v);
        } else {
            auto v = readRealVec(f);
            setVector(v);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Exception: %1").arg(e.what()));
    }
}
