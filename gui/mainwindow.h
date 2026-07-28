#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>

#include "matrix_widget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    // ── Tab setup methods ──
    void setupMatrixArithmeticTab();
    void setupVectorArithmeticTab();
    void setupCoreOpsTab();
    void setupNormsTab();
    void setupLinearSystemTab();
    void setupEigenTab();
    void setupLSFittingTab();
    void setupBenchmarkTab();

    // ── Computation methods (one per operation) ──
    // Section 1: Matrix Arithmetic
    void computeMatrixAdd();
    void computeMatrixSub();
    void computeMatrixMul();
    void computeMatrixScalarOp();
    void computeMatrixVectorMul();

    // Section 2: Vector Arithmetic
    void computeVectorAdd();
    void computeVectorSub();
    void computeVectorDot();
    void computeVectorCross();
    void computeVectorScalarMul();
    void computeVectorNorms();

    // Section 3: Core Matrix Operations
    void computeDeterminant();
    void computeAdjugateInverse();
    void computeTranspose();
    void computeRankRREF();

    // Section 4: Norms & Condition
    void computeFrobeniusNorm();
    void computeRowNorm();
    void computeColNorm();
    void computeSpectralNorm();
    void computeConditionNumber();

    // Section 5: Linear System
    void computeLinearSystem();

    // Section 6: Eigen
    void computeEigen();

    // Section 7: Least Squares
    void computeLeastSquares();

    // Section 8: Benchmark
    void computeBenchmark();

    // ── Helper: show result in a table ──
    void showMatrixInTable(QTableWidget* tbl, const Matrix<double>& mat);
    void showMatrixInTable(QTableWidget* tbl, const Matrix<Complex>& mat);
    void showVectorInTable(QTableWidget* tbl, const Vector<double>& vec);
    void showVectorInTable(QTableWidget* tbl, const Vector<Complex>& vec);

    // ── Helper: append text to result text edit ──
    void appendResultText(const QString& text);
    void appendResultSep();

    // ── Shared result display ──
    QTableWidget* createResultTable(int rows, int cols);
    void setResultTableComplex(QTableWidget* tbl, bool complex);

    // ── Convenience helper ──
    template<typename Func>
    void safeCompute(Func&& fn);

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 1: Matrix Arithmetic
    // ═══════════════════════════════════════════════════════
    // 1.1 Add
    MatrixInputWidget* m_addA;
    MatrixInputWidget* m_addB;
    QTableWidget*      m_addResult;
    QPushButton*       m_addBtn;
    // 1.2 Sub
    MatrixInputWidget* m_subA;
    MatrixInputWidget* m_subB;
    QTableWidget*      m_subResult;
    QPushButton*       m_subBtn;
    // 1.3 Mul
    MatrixInputWidget* m_mulA;
    MatrixInputWidget* m_mulB;
    QTableWidget*      m_mulResult;
    QPushButton*       m_mulBtn;
    // 1.4 Scalar
    MatrixInputWidget* m_scalarA;
    QDoubleSpinBox*    m_scalarSpinReal;
    QLineEdit*          m_scalarImagEdit;
    QWidget*           m_scalarComplexRow;
    QTableWidget*      m_scalarMulResult;
    QTableWidget*      m_scalarDivResult;
    QPushButton*       m_scalarBtn;
    // 1.5 Mat × Vec
    MatrixInputWidget* m_mvA;
    VectorInputWidget* m_mvV;
    QTableWidget*      m_mvResult;
    QPushButton*       m_mvBtn;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 2: Vector Arithmetic
    // ═══════════════════════════════════════════════════════
    VectorInputWidget *m_vAdd1, *m_vAdd2;
    QTableWidget*      m_vAddResult;
    VectorInputWidget *m_vSub1, *m_vSub2;
    QTableWidget*      m_vSubResult;
    VectorInputWidget *m_vDot1, *m_vDot2;
    QLabel*            m_vDotResult;
    VectorInputWidget *m_vCross1, *m_vCross2;
    QTableWidget*      m_vCrossResult;
    VectorInputWidget* m_vScalarV;
    QDoubleSpinBox*    m_vScalarSpin;
    QTableWidget*      m_vScalarResult;
    VectorInputWidget* m_vNormsVec;
    QTextEdit*         m_vNormsResult;
    QDoubleSpinBox*    m_vNormsP;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 3: Core Matrix Operations
    // ═══════════════════════════════════════════════════════
    MatrixInputWidget* m_detA;
    QLabel*            m_detResult;
    MatrixInputWidget* m_adjA;
    QTableWidget*      m_adjResult;
    QTableWidget*      m_invResult;
    QLabel*            m_adjDetLabel;
    QLabel*            m_adjInvLabel;
    MatrixInputWidget* m_transA;
    QTableWidget*      m_transResult;
    MatrixInputWidget* m_rrefA;
    QLabel*            m_rankResult;
    QTableWidget*      m_rrefResult;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 4: Norms & Condition
    // ═══════════════════════════════════════════════════════
    MatrixInputWidget* m_frobA;
    QLabel*            m_frobResult;
    MatrixInputWidget* m_rowA;
    QLabel*            m_rowResult;
    MatrixInputWidget* m_colA;
    QLabel*            m_colResult;
    MatrixInputWidget* m_specA;
    QLabel*            m_specResult;
    MatrixInputWidget* m_condA;
    QLabel*            m_condResult;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 5: Linear System
    // ═══════════════════════════════════════════════════════
    MatrixInputWidget* m_lsA;
    VectorInputWidget* m_lsB;
    QTableWidget*      m_lsResult;
    QLabel*            m_lsStatus;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 6: Eigen
    // ═══════════════════════════════════════════════════════
    MatrixInputWidget* m_eigenA;
    QTextEdit*         m_eigenResult;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 7: Least Squares
    // ═══════════════════════════════════════════════════════
    QTableWidget*      m_lsPointsTable;
    QSpinBox*          m_lsDegree;
    QTextEdit*         m_lsFitResult;

    // ═══════════════════════════════════════════════════════
    // UI Widgets — Section 8: Benchmark
    // ═══════════════════════════════════════════════════════
    QTextEdit*         m_benchResult;
    QPushButton*       m_benchBtn;
    QProgressBar*      m_benchProgress;

    // Main tab widget
    QTabWidget* m_mainTabs;
};

#endif // MAINWINDOW_H
