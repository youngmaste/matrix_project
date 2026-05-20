#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include "matrix.h"
#include "vector.h"
#include <thread>
#include <cmath>
#include <algorithm>
#include <complex>

const double EPS = 1e-10;

inline double absT(int x) { return std::abs(x); }
inline double absT(long x) { return std::abs(x); }
inline double absT(float x) { return std::abs(x); }
inline double absT(double x) { return std::fabs(x); }
inline double absT(const std::complex<double>& x) { return std::abs(x); }

template <typename T>
Matrix<T> transpose(const Matrix<T>& m) {
    Matrix<T> res(m.cols(), m.rows());
    for (int i = 0; i < m.rows(); ++i)
        for (int j = 0; j < m.cols(); ++j)
            res(j, i) = m(i, j);
    return res;
}

template <typename T>
int rankMatrix(Matrix<T> m) {
    int r = 0, n = m.rows(), k = m.cols();
    for (int col = 0; col < k && r < n; ++col) {
        int pivot = r;
        for (int i = r; i < n; ++i)
            if (absT(m(i, col)) > absT(m(pivot, col))) pivot = i;
        if (absT(m(pivot, col)) < EPS) continue;
        
        m.swapRows(r, pivot);

        for (int i = r+1; i < n; ++i) {
            T f = m(i, col) / m(r, col);
            for (int j = col; j < k; ++j)
                m(i, j) -= f * m(r, j);
        }
        r++;
    }
    return r;
}

template <typename T>
T determinant(Matrix<T> m) {
    if (m.rows() != m.cols())
        throw std::invalid_argument("Determinant only supports square matrix");
    int n = m.rows();
    T det = 1;
    for (int i = 0; i < n; ++i) {
        int pivot = i;
        for (int j = i; j < n; ++j)
            if (absT(m(j, i)) > absT(m(pivot, i))) pivot = j;
        if (absT(m(pivot, i)) < EPS) return 0;
        
        if (pivot != i) {
            m.swapRows(i, pivot);
            det *= -1;
        }

        det *= m(i, i);
        for (int j = i+1; j < n; ++j) {
            T f = m(j, i) / m(i, i);
            for (int k = i; k < n; ++k)
                m(j, k) -= f * m(i, k);
        }
    }
    return det;
}

template <typename T>
Matrix<T> inverse(const Matrix<T>& m) {
    if (m.rows() != m.cols())
        throw std::invalid_argument("Inverse only supports square matrix");
    int n = m.rows();
    T det = determinant(m);
    if (absT(det) < EPS)
        throw std::invalid_argument("Matrix is singular, cannot invert");

    Matrix<T> aug(n, 2*n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) aug(i, j) = m(i, j);
        aug(i, i+n) = 1;
    }

    for (int i = 0; i < n; ++i) {
        int pivot = i;
        for (int j = i; j < n; ++j)
            if (absT(aug(j, i)) > absT(aug(pivot, i))) pivot = j;
        
        aug.swapRows(i, pivot);
        
        T div = aug(i, i);
        for (int j = i; j < 2*n; ++j) aug(i, j) /= div;
        
        for (int j = 0; j < n; ++j)
            if (j != i && absT(aug(j, i)) > EPS) {
                T f = aug(j, i);
                for (int k = i; k < 2*n; ++k)
                    aug(j, k) -= f * aug(i, k);
            }
    }

    Matrix<T> inv(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            inv(i, j) = aug(i, j+n);
    return inv;
}

template <typename T>
double frobeniusNorm(const Matrix<T>& m) {
    double sum = 0;
    for (int i = 0; i < m.rows(); ++i)
        for (int j = 0; j < m.cols(); ++j)
            sum += pow(absT(m(i,j)), 2);
    return sqrt(sum);
}

template <typename T>
double rowSumNorm(const Matrix<T>& m) {
    double max_sum = 0;
    for (int i = 0; i < m.rows(); ++i) {
        double s = 0;
        for (int j = 0; j < m.cols(); ++j) s += absT(m(i,j));
        max_sum = std::max(max_sum, s);
    }
    return max_sum;
}

template <typename T>
double colSumNorm(const Matrix<T>& m) {
    double max_sum = 0;
    for (int j = 0; j < m.cols(); ++j) {
        double s = 0;
        for (int i = 0; i < m.rows(); ++i) s += absT(m(i,j));
        max_sum = std::max(max_sum, s);
    }
    return max_sum;
}

template <typename T>
double conditionNumber(const Matrix<T>& m) {
    try {
        Matrix<T> inv = inverse(m);
        return frobeniusNorm(m) * frobeniusNorm(inv);
    } catch (...) {
        return INFINITY;
    }
}

template <typename T>
Vector<T> solveLU(const Matrix<T>& A, const Vector<T>& b) {
    if (A.rows() != A.cols() || A.rows() != b.rows())
        throw std::invalid_argument("Dimension mismatch");
    int n = A.rows();
    Matrix<T> L(n, n), U = A;
    for (int i = 0; i < n; ++i) L(i, i) = 1;

    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            T f = U(j, i) / U(i, i);
            L(j, i) = f;
            for (int k = i; k < n; ++k) U(j, k) -= f * U(i, k);
        }
    }

    Vector<T> y(n);
    for (int i = 0; i < n; ++i) {
        y(i,0) = b(i,0);
        for (int j = 0; j < i; ++j) y(i,0) -= L(i,j)*y(j,0);
    }

    Vector<T> x(n);
    for (int i = n-1; i >= 0; --i) {
        x(i,0) = y(i,0);
        for (int j = i+1; j < n; ++j) x(i,0) -= U(i,j)*x(j,0);
        x(i,0) /= U(i,i);
    }
    return x;
}

template <typename T>
Vector<T> leastSquares(const Matrix<T>& A, const Vector<T>& b) {
    Matrix<T> At = transpose(A);
    Matrix<T> AtA = At * A;
    Vector<T> Atb = Vector<T>(At * b);
    return solveLU(AtA, Atb);
}

template <typename T>
double eigenValuePower(const Matrix<T>& m, int iter = 1000) {
    if (m.rows() != m.cols())
        throw std::invalid_argument("Eigenvalue only supports square matrix");
    int n = m.rows();
    Vector<T> v(n);
    for (int i = 0; i < n; ++i) v(i,0) = 1;
    for (int i = 0; i < iter; ++i) {
        Vector<T> w = Vector<T>(m * v);
        v = w * (1.0 / w.norm());
    }
    Vector<T> mv = Vector<T>(m * v);
    return mv.dot(v) / v.dot(v);
}

void multiplyBlock(const Matrix<double>& a, const Matrix<double>& b, Matrix<double>& res, int start, int end) {
    for (int i = start; i < end; ++i)
        for (int k = 0; k < a.cols(); ++k)
            for (int j = 0; j < b.cols(); ++j)
                res(i,j) += a(i,k)*b(k,j);
}

Matrix<double> mulThread(const Matrix<double>& a, const Matrix<double>& b, int threads = 4) {
    if (a.cols() != b.rows()) throw std::invalid_argument("Dimension mismatch");
    Matrix<double> res(a.rows(), b.cols());
    int chunk = a.rows() / threads;
    std::vector<std::thread> t;

    for (int i = 0; i < threads; ++i) {
        int s = i * chunk;
        int e = (i == threads-1) ? a.rows() : (i+1)*chunk;
        t.emplace_back(multiplyBlock, std::ref(a), std::ref(b), std::ref(res), s, e);
    }
    for (auto& th : t) th.join();
    return res;
}

Matrix<double> PCA(const Matrix<double>& data, int k) {
    int n = data.rows(), m = data.cols();
    Matrix<double> mean(1, m);
    for (int j = 0; j < m; ++j)
        for (int i = 0; i < n; ++i) mean(0,j) += data(i,j);
    for (int j = 0; j < m; ++j) mean(0,j) /= n;

    Matrix<double> centered(n, m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) centered(i,j) = data(i,j) - mean(0,j);

    Matrix<double> cov = transpose(centered) * centered * (1.0/(n-1));
    Matrix<double> res(n, k);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < k; ++j)
            res(i,j) = centered(i,j);
    return res;
}

#endif