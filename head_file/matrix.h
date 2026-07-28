#ifndef MATRIX_H
#define MATRIX_H

#include "matrix_base.h"
#include "vector.h"  
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <complex>
#include <stdexcept>
#include <cmath>
#include <algorithm>

template <typename T>
class Matrix : public MatrixBase<T> {
private:
    int _rows;
    int _cols;
    T** data;

    void allocate(int r, int c) {
        _rows = r;
        _cols = c;
        data = new T*[r];
        for (int i = 0; i < r; ++i) {
            data[i] = new T[c]();
        }
    }

    void free() {
        if (data) {
            for (int i = 0; i < _rows; ++i) delete[] data[i];
            delete[] data;
            data = nullptr;
        }
        _rows = _cols = 0;
    }

public:
    Matrix(int r = 0, int c = 0) {
        if (r < 0 || c < 0) throw std::invalid_argument("Dimensions cannot be negative");
        allocate(r, c);
    }

    Matrix(const Matrix<T>& other) {
        allocate(other._rows, other._cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                data[i][j] = other.data[i][j];
    }

    ~Matrix() override {
        free();
    }

    Matrix<T>& operator=(const Matrix<T>& other) {
        if (this == &other) return *this;
        free();
        allocate(other._rows, other._cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                data[i][j] = other.data[i][j];
        return *this;
    }

    int rows() const override { return _rows; }
    int cols() const override { return _cols; }

    T& operator()(int i, int j) override {
        if (i < 0 || i >= _rows || j < 0 || j >= _cols)
            throw std::out_of_range("Matrix index out of range");
        return data[i][j];
    }

    const T& operator()(int i, int j) const override {
        if (i < 0 || i >= _rows || j < 0 || j >= _cols)
            throw std::out_of_range("Matrix index out of range");
        return data[i][j];
    }

    void swapRows(int i, int j) {
        if (i < 0 || i >= _rows || j < 0 || j >= _rows)
            throw std::out_of_range("Row index out of range");
        std::swap(data[i], data[j]);
    }

    void print() const override {
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                std::cout << std::setw(12) << data[i][j];
            }
            std::cout << "\n";
        }
    }

    Matrix<T> operator+(const Matrix<T>& other) const {
        if (_rows != other._rows || _cols != other._cols)
            throw std::invalid_argument("Dimension mismatch");
        Matrix<T> res(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                res(i, j) = data[i][j] + other(i, j);
        return res;
    }

    Matrix<T> operator-(const Matrix<T>& other) const {
        if (_rows != other._rows || _cols != other._cols)
            throw std::invalid_argument("Dimension mismatch");
        Matrix<T> res(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                res(i, j) = data[i][j] - other(i, j);
        return res;
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        if (_cols != other._rows)
            throw std::invalid_argument("Dimension mismatch");
        Matrix<T> res(_rows, other._cols);
        for (int i = 0; i < _rows; ++i)
            for (int k = 0; k < _cols; ++k)
                for (int j = 0; j < other._cols; ++j)
                    res(i, j) += data[i][k] * other(k, j);
        return res;
    }

    Matrix<T> operator*(T scalar) const {
        Matrix<T> res(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                res(i, j) = data[i][j] * scalar;
        return res;
    }

    Matrix<T> operator/(T scalar) const {
        if (scalar == T(0))
            throw std::invalid_argument("Division by zero scalar");
        Matrix<T> res(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                res(i, j) = data[i][j] / scalar;
        return res;
    }

    bool operator==(const Matrix<T>& other) const {
        if (_rows != other._rows || _cols != other._cols) return false;
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                if (data[i][j] != other(i, j)) return false;
        return true;
    }

    bool operator!=(const Matrix<T>& other) const {
        return !(*this == other);
    }

    Matrix<T> transpose() const {
        Matrix<T> res(_cols, _rows);
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                res(j, i) = data[i][j];
            }
        }
        return res;
    }

    T determinant() const {
        if (_rows != _cols)
            throw std::invalid_argument("Determinant only for square matrix");
        
        int n = _rows;
        if (n == 0) return T(1);
        
        Matrix<T> mat = *this;
        T det = T(1);
        const double eps = 1e-10;

        for (int col = 0; col < n; ++col) {
            int pivot = col;
            for (int r = col; r < n; ++r) {
                if (std::abs(mat(r, col)) > std::abs(mat(pivot, col)))
                    pivot = r;
            }

            if (pivot != col) {
                mat.swapRows(col, pivot);
                det = -det;
            }

            T pivot_val = mat(col, col);
            if (std::abs(pivot_val) < eps)
                return T(0);

            det *= pivot_val;

            for (int r = col + 1; r < n; ++r) {
                T factor = mat(r, col) / pivot_val;
                for (int c = col; c < n; ++c) {
                    mat(r, c) -= factor * mat(col, c);
                }
            }
        }
        return det;
    }

    int rank() const override {
        Matrix<T> mat = *this;
        int n = _rows, m = _cols;
        int rank_val = 0;
        const double eps = 1e-10;

        for (int col = 0, row = 0; col < m && row < n; ++col) {
            int pivot = row;
            for (int i = row; i < n; ++i) {
                if (std::abs(mat(i, col)) > std::abs(mat(pivot, col)))
                    pivot = i;
            }
            if (std::abs(mat(pivot, col)) < eps) continue;

            mat.swapRows(row, pivot);

            for (int i = 0; i < n; ++i) {
                if (i != row && std::abs(mat(i, col)) > eps) {
                    T factor = mat(i, col) / mat(row, col);
                    for (int j = col; j < m; ++j)
                        mat(i, j) -= factor * mat(row, j);
                }
            }
            row++;
            rank_val++;
        }
        return rank_val;
    }

    Matrix<T> rref() const {
        Matrix<T> mat = *this;
        int n = _rows, m = _cols;
        const double eps = 1e-10;
        int current_row = 0;

        for (int col = 0; col < m && current_row < n; ++col) {
            int pivot = current_row;
            for (int i = current_row; i < n; ++i) {
                if (std::abs(mat(i, col)) > std::abs(mat(pivot, col)))
                    pivot = i;
            }
            if (std::abs(mat(pivot, col)) < eps) continue;

            mat.swapRows(current_row, pivot);

            T div = mat(current_row, col);
            for (int j = col; j < m; ++j)
                mat(current_row, j) /= div;

            for (int i = 0; i < n; ++i) {
                if (i != current_row && std::abs(mat(i, col)) > eps) {
                    T factor = mat(i, col);
                    for (int j = col; j < m; ++j)
                        mat(i, j) -= factor * mat(current_row, j);
                }
            }
            current_row++;
        }
        return mat;
    }

    T minor(int i, int j) const {
        if (_rows != _cols)
            throw std::invalid_argument("Minor only for square matrix");
        int n = _rows;
        Matrix<T> sub(n-1, n-1);
        int x = 0, y = 0;
        for (int r = 0; r < n; r++) {
            if (r == i) continue;
            y = 0;
            for (int c = 0; c < n; c++) {
                if (c == j) continue;
                sub(x, y) = data[r][c];
                y++;
            }
            x++;
        }
        return sub.determinant();
    }

    T cofactor(int i, int j) const {
        return ((i+j) % 2 == 0 ? T(1) : T(-1)) * minor(i, j);
    }

    Matrix<T> adjugate() const {
        if (_rows != _cols)
            throw std::invalid_argument("Adjugate only for square matrix");
        int n = _rows;
        Matrix<T> adj(n, n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                adj(i, j) = cofactor(j, i);
            }
        }
        return adj;
    }

    Matrix<T> inverse() const {
        T det = determinant();
        const double eps = 1e-10;
        if (std::abs(det) < eps)
            throw std::runtime_error("Matrix is singular, cannot invert");
        
        Matrix<T> adj = adjugate();
        Matrix<T> inv(_rows, _cols);
        for (int i = 0; i < _rows; i++) {
            for (int j = 0; j < _cols; j++) {
                inv(i, j) = adj(i, j) / det;
            }
        }
        return inv;
    }

    Matrix<T> conjTranspose() const
    {
        Matrix<T> res(_cols, _rows);
        for (int i = 0; i < _rows; ++i) {
            for (int j = 0; j < _cols; ++j) {
                if constexpr (std::is_same_v<T, std::complex<double>>) {
                    res(j, i) = std::conj(data[i][j]);
                } else {
                    res(j, i) = data[i][j];
                }
            }
        }
        return res;
    }

    Vector<T> solveLinear(const Vector<T>& b) const {
        int m = _rows;
        int n = _cols;

        if (b.rows() != m) {
            throw std::invalid_argument("Dimension mismatch: A.rows() != b.rows()");
        }

        if (m == n) {
            Matrix<T> aug(m, n + 1);
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j)
                    aug(i, j) = data[i][j];
                aug(i, n) = b(i, 0);
            }

            Matrix<T> rrefMat = aug.rref();
            Vector<T> x(n);

            for (int i = 0; i < n; ++i) {
                x(i, 0) = rrefMat(i, n);

                bool pivotFound = false;
                for (int j = 0; j < n; ++j) {
                    if (std::abs(rrefMat(i, j)) > 1e-10) {
                        pivotFound = true;
                        break;
                    }
                }
                if (!pivotFound && std::abs(rrefMat(i, n)) > 1e-10) {
                    return Vector<T>();
                }
            }
            return x;
        } else {
            Matrix<T> At = this->transpose();
            Matrix<T> AtA = At * (*this);
            Vector<T> Atb = At * b;
            Matrix<T> aug2(AtA.rows(), AtA.cols() + 1);

            for (int i = 0; i < AtA.rows(); ++i) {
                for (int j = 0; j < AtA.cols(); ++j)
                    aug2(i, j) = AtA(i, j);
                aug2(i, AtA.cols()) = Atb(i, 0);
            }

            Matrix<T> rref2 = aug2.rref();
            Vector<T> x_ls(n);
            for (int i = 0; i < n; ++i)
                x_ls(i, 0) = rref2(i, n);
            return x_ls;
        }
    }

    double norm_frobenius() const override {
        double sum = 0.0;
        int rows = _rows;
        int cols = _cols;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                double abs_val = std::abs(data[i][j]);
                sum += abs_val * abs_val;
            }
        }
        return sqrt(sum);
    }

    double norm_row() const
    {
        double max_sum = 0.0;
        int rows = _rows;
        int cols = _cols;

        for (int i = 0; i < rows; ++i)
        {
            double row_sum = 0.0;
            for (int j = 0; j < cols; ++j)
            {
                row_sum += std::abs(data[i][j]);
            }
            if(row_sum > max_sum)
                max_sum = row_sum;
        }
        return max_sum;
    }

    double norm_col() const {
        double max_sum = 0.0;
        int rows = _rows;
        int cols = _cols;

        for (int j = 0; j < cols; ++j) {
            double col_sum = 0.0;
            for (int i = 0; i < rows; ++i) {
                col_sum += std::abs(data[i][j]);
            }
            if (col_sum > max_sum) {
                max_sum = col_sum;
            }
        }
        return max_sum;
    }

    double norm_spectral(int iter = 100, double tol = 1e-8) const
    {
        if (_rows == 0 || _cols == 0)
            throw std::invalid_argument("Empty matrix!");

        Matrix<T> AH = conjTranspose();
        Matrix<T> B = AH * (*this);
        int n = B.rows();

        Vector<T> x(n);
        for (int i = 0; i < n; ++i)
            x(i, 0) = T(1.0);

        for (int k = 0; k < iter; ++k) {
            Vector<T> y = B * x;
            double nrm = y.norm_l2();
            if (nrm < tol) break;

            for (int i = 0; i < n; ++i)
                y(i, 0) = y(i, 0) / nrm;

            double diff = 0.0;
            for (int i = 0; i < n; ++i)
                diff += std::abs(y(i, 0) - x(i, 0));
            if (diff < tol) break;

            x = y;
        }

        Vector<T> tmp = B * x;
        double lambda = 0.0;
        for (int i = 0; i < n; ++i) {
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                lambda += std::real(std::conj(x(i, 0)) * tmp(i, 0));
            } else {
                lambda += x(i, 0) * tmp(i, 0);
            }
        }

        return sqrt(fabs(lambda));
    }

    double conditionNumber() const {
        if (_rows != _cols) {
            return HUGE_VAL;
        }

        T det = determinant();
        const double eps = 1e-10;
        if (std::abs(det) < eps) {
            return HUGE_VAL;
        }

        Matrix<T> A_inv = inverse();
        double nA = norm_col();
        double nAi = A_inv.norm_col();

        return nA * nAi;
    }

    // ===================== 特征值/特征向量（完全适配vector.h） ======================
    using Complex = std::complex<double>;
    const double EIGEN_EPS = 1e-8;

    template<typename U>
    Matrix<U> cast() const {
        Matrix<U> res(_rows, _cols);
        for (int i = 0; i < _rows; ++i)
            for (int j = 0; j < _cols; ++j)
                res(i, j) = static_cast<U>(data[i][j]);
        return res;
    }

    std::vector<Complex> eigenvalues() const {
        if (_rows != _cols)
            throw std::invalid_argument("Eigenvalues only for square matrix!");
        int n = _rows;
        std::vector<Complex> eig(n);

        if constexpr (std::is_same_v<T, Complex>) {
            Matrix<Complex> A = this->template cast<Complex>();
            for (int iter = 0; iter < 1000; ++iter) {
                bool ok = true;
                for (int i = 1; i < n; ++i)
                    if (std::abs(A(i,i-1)) > EIGEN_EPS) ok = false;
                if (ok) break;

                for (int k=0;k<n-1;k++){
                    Vector<Complex> v(n);
                    // 核心修复：v(i) → v(i,0) 适配vector.h
                    for(int i=0;i<n;i++) v(i,0)=A(i,k);
                    double nrm=0;
                    for(int i=k;i<n;i++) nrm+=std::norm(v(i,0));
                    nrm=std::sqrt(nrm);
                    Complex s = v(k,0)/std::abs(v(k,0));
                    v(k,0)-=s*nrm;
                    double vn=0;
                    for(int i=k;i<n;i++) vn+=std::norm(v(i,0));
                    vn=std::sqrt(vn);
                    if(vn< EIGEN_EPS) continue;
                    for(int i=k;i<n;i++) v(i,0)/=vn;

                    Matrix<Complex> H(n,n);
                    for(int i=0;i<n;i++) H(i,i)=1;
                    for(int i=k;i<n;i++)
                        for(int j=k;j<n;j++)
                            H(i,j)-=2.0*v(i,0)*std::conj(v(j,0));
                    A=H*A;
                }
            }
            for(int i=0;i<n;i++) eig[i]=A(i,i);
        } else {
            Matrix<double> A = this->template cast<double>();
            for(int iter=0;iter<1000;iter++){
                bool ok=true;
                for(int i=1;i<n;i++)
                    if(std::abs(A(i,i-1))>EIGEN_EPS) ok=false;
                if(ok) break;

                for(int k=0;k<n-1;k++){
                    Vector<double> v(n);
                    //  核心修复：v(i) → v(i,0) 适配vector.h
                    for(int i=0;i<n;i++) v(i,0)=A(i,k);
                    double nrm=0;
                    for(int i=k;i<n;i++) nrm+=v(i,0)*v(i,0);
                    nrm=std::sqrt(nrm);
                    double s = (v(k,0)>=0) ? -1 : 1;
                    v(k,0)-=s*nrm;
                    double vn=0;
                    for(int i=k;i<n;i++) vn+=v(i,0)*v(i,0);
                    vn=std::sqrt(vn);
                    if(vn<EIGEN_EPS) continue;
                    for(int i=k;i<n;i++) v(i,0)/=vn;

                    Matrix<double> H(n,n);
                    for(int i=0;i<n;i++) H(i,i)=1;
                    for(int i=k;i<n;i++)
                        for(int j=k;j<n;j++)
                            H(i,j)-=2.0*v(i,0)*v(j,0);
                    A=H*A;
                }
            }

            int i=0;
            while(i<n){
                if(i==n-1 || std::abs(A(i+1,i))<EIGEN_EPS){
                    eig[i]=Complex(A(i,i),0);
                    i++;
                } else {
                    double a=A(i,i),b=A(i,i+1);
                    double c=A(i+1,i),d=A(i+1,i+1);
                    double tr=a+d,det=a*d-b*c;
                    double disc=tr*tr-4*det;
                    if(disc>=-EIGEN_EPS){
                        disc=std::fabs(disc);
                        eig[i]=(tr+std::sqrt(disc))/2.0;
                        eig[i+1]=(tr-std::sqrt(disc))/2.0;
                    } else {
                        eig[i]=Complex(tr/2,std::sqrt(-disc)/2);
                        eig[i+1]=Complex(tr/2,-std::sqrt(-disc)/2);
                    }
                    i+=2;
                }
            }
        }
        return eig;
    }

    Vector<Complex> eigenVector(const Complex& lam) const {
        int n = _rows;
        Matrix<Complex> A = this->template cast<Complex>();
        Matrix<Complex> L(n,n);
        for(int i=0;i<n;i++) L(i,i)=lam;
        auto r=(A-L).rref();

        // Find pivot columns in RREF
        std::vector<int> pivotCol(n, -1);
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(abs(r(i,j)) > 1e-10){ pivotCol[i] = j; break; }
            }
        }

        // Find free columns (non-pivot)
        std::vector<bool> isPivot(n, false);
        for(int i=0;i<n;i++)
            if(pivotCol[i] >= 0) isPivot[pivotCol[i]] = true;

        int freeCol = -1;
        for(int j=n-1;j>=0;j--)
            if(!isPivot[j]){ freeCol = j; break; }

        Vector<Complex> v(n);
        if(freeCol < 0){ for(int i=0;i<n;i++) v(i,0)=Complex(0,0); return v; }

        v(freeCol,0)=Complex(1,0);
        for(int i=n-1;i>=0;i--){
            int pc = pivotCol[i];
            if(pc < 0) continue;
            Complex sum=0;
            for(int j=pc+1;j<n;j++) sum += r(i,j) * v(j,0);
            v(pc,0) = -sum;
        }
        return v;
    }

    bool isDiagonalizable() const {
        auto eigs = eigenvalues();
        int n = _rows;
        Matrix<Complex> A = this->template cast<Complex>();

        for(auto lam : eigs){
            Matrix<Complex> L(n,n);
            for(int i=0;i<n;i++) L(i,i)=lam;
            int r = (A-L).rank();
            int cnt=0;
            for(auto x:eigs) if(std::abs(x-lam)<EIGEN_EPS) cnt++;
            if(n-r != cnt) return false;
        }
        return true;
    }
    // ================================================================================

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T>& m) {
        for (int i = 0; i < m._rows; ++i) {
            for (int j = 0; j < m._cols; ++j)
                os << std::setw(12) << m(i, j);
            os << "\n";
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Matrix<T>& m) {
        for (int i = 0; i < m.rows(); ++i)
            for (int j = 0; j < m.cols(); ++j)
                is >> m(i, j);
        return is;
    }
};

#endif