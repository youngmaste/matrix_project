#ifndef MATRIX_WIDGET_H
#define MATRIX_WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <complex>

#include "head_file/matrix.h"
#include "head_file/vector.h"
#include "head_file/utils.h"

using Complex = std::complex<double>;

// ============================================================================
// MatrixInputWidget — reusable panel: dimensions + type + table + file loader
// ============================================================================
class MatrixInputWidget : public QWidget {
    Q_OBJECT
public:
    explicit MatrixInputWidget(const QString& title, QWidget* parent = nullptr);

    // Configure dimensions
    void setDimensions(int rows, int cols);
    int rows() const;
    int cols() const;

    // Type
    bool isComplex() const;

    // Get matrix data
    Matrix<double>  getRealMatrix() const;
    Matrix<Complex> getComplexMatrix() const;

    // Set matrix data (for displaying results)
    void setMatrix(const Matrix<double>& mat);
    void setMatrix(const Matrix<Complex>& mat);
    void setVector(const Vector<double>& vec);
    void setVector(const Vector<Complex>& vec);

    // File loading
    void loadFromFile();

    // Resize table
    void resizeTable(int rows, int cols);

signals:
    void matrixChanged();

private slots:
    void onDimensionsChanged();
    void onTypeChanged(int index);

private:
    void buildUI(const QString& title);
    void syncTableDimensions();

    QSpinBox*      m_rowsSpin;
    QSpinBox*      m_colsSpin;
    QComboBox*     m_typeCombo;     // 0=Real, 1=Complex
    QTableWidget*  m_table;
    QPushButton*   m_loadBtn;
    bool           m_updating = false;  // prevent recursive signals
};

// ============================================================================
// VectorInputWidget — for standalone vector input (not column of matrix)
// ============================================================================
class VectorInputWidget : public QWidget {
    Q_OBJECT
public:
    explicit VectorInputWidget(const QString& title, QWidget* parent = nullptr);

    void setDimension(int dim);
    int  dimension() const;
    bool isComplex() const;

    Vector<double>  getRealVector() const;
    Vector<Complex> getComplexVector() const;

    void setVector(const Vector<double>& vec);
    void setVector(const Vector<Complex>& vec);

    void loadFromFile();

signals:
    void vectorChanged();

private slots:
    void onDimChanged();
    void onTypeChanged(int index);

private:
    void buildUI(const QString& title);
    void syncTableDimensions();

    QSpinBox*      m_dimSpin;
    QComboBox*     m_typeCombo;
    QTableWidget*  m_table;
    QPushButton*   m_loadBtn;
    bool           m_updating = false;
};

#endif // MATRIX_WIDGET_H
