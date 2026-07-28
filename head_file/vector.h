#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <complex>
#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <iomanip>

// 关键修复：前置声明 Matrix，避免循环依赖
template <typename T>
class Matrix;

// 继承前置声明的 Matrix
template <typename T>
class Vector : public Matrix<T> {
public:
    // 保留你的默认构造函数
    Vector() = default;

    // 保留你的维度构造
    Vector(int n) : Matrix<T>(n, 1) {}

    // 保留你的矩阵转向量构造（修复小bug）
    Vector(const Matrix<T>& mat) {
        if (mat.cols() != 1)
            throw std::invalid_argument("Only column matrix can construct vector");
        *this = Vector<T>(mat.rows());
        for (int i = 0; i < this->rows(); ++i)
            (*this)(i, 0) = mat(i, 0);
    }

    // ===================== 保留你所有的原有函数 =====================
    // 点积（完全保留）
    T dot(const Vector<T>& other) const {
        if (this->rows() != other.rows())
            throw std::invalid_argument("Dot product requires vectors with SAME dimension!");
        
        T res = T(0);
        int n = this->rows();
        for (int i = 0; i < n; ++i) {
            if constexpr (std::is_same_v<T, std::complex<double>>) {
                res += (*this)(i, 0) * std::conj(other(i, 0));
            } else {
                res += (*this)(i, 0) * other(i, 0);
            }
        }
        return res;
    }

    // 叉乘（完全保留）
    Vector<T> cross(const Vector<T>& other) const {
        if (this->rows() != 3 || other.rows() != 3) {
            throw std::invalid_argument("Cross product ONLY supports 3D vectors!");
        }

        Vector<T> res(3);
        res(0, 0) = (*this)(1, 0) * other(2, 0) - (*this)(2, 0) * other(1, 0);
        res(1, 0) = (*this)(2, 0) * other(0, 0) - (*this)(0, 0) * other(2, 0);
        res(2, 0) = (*this)(0, 0) * other(1, 0) - (*this)(1, 0) * other(0, 0);

        return res;
    }

    // 向量加法（完全保留）
    Vector<T> operator+(const Vector<T>& other) const {
        if (this->rows() != other.rows())
            throw std::invalid_argument("Vector addition requires vectors with SAME dimension!");
        
        Vector<T> res(this->rows());
        for (int i = 0; i < this->rows(); ++i) {
            res(i, 0) = (*this)(i, 0) + other(i, 0);
        }
        return res;
    }

    // 向量减法（完全保留）
    Vector<T> operator-(const Vector<T>& other) const {
        if (this->rows() != other.rows())
            throw std::invalid_argument("Vector subtraction requires vectors with SAME dimension!");
        
        Vector<T> res(this->rows());
        for (int i = 0; i < this->rows(); ++i) {
            res(i, 0) = (*this)(i, 0) - other(i, 0);
        }
        return res;
    }

    // 向量数乘（完全保留）
    Vector<T> operator*(const T& scalar) const {
        Vector<T> res(this->rows());
        for (int i = 0; i < this->rows(); ++i) {
            res(i, 0) = (*this)(i, 0) * scalar;
        }
        return res;
    }

    // 标量*向量（完全保留）
    friend Vector<T> operator*(const T& scalar, const Vector<T>& vec) {
        return vec * scalar;
    }

    // 所有范数（完全保留）
    double norm_p(double p) const {
        if (p < 1.0)
            throw std::invalid_argument("p-norm requires p >= 1");
        
        double sum = 0.0;
        int n = this->rows();
        for (int i = 0; i < n; ++i) {
            double abs_val = std::abs((*this)(i, 0));
            sum += pow(abs_val, p);
        }
        return pow(sum, 1.0 / p);
    }

    double norm_inf() const {
        double max_val = 0.0;
        int n = this->rows();
        for (int i = 0; i < n; ++i) {
            double abs_val = std::abs((*this)(i, 0));
            if (abs_val > max_val)
                max_val = abs_val;
        }
        return max_val;
    }

    double norm_l1() const {
        return norm_p(1.0);
    }

    double norm_l2() const {
        return norm_p(2.0);
    }
    // ==============================================================

    //  核心修复：新增向量输出运算符（主程序打印特征向量必须用）
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& v) {
        for (int i = 0; i < v.rows(); ++i)
            os << std::setw(12) << v(i, 0) << "\n";
        return os;
    }
};

//  新增 矩阵*向量 运算符（矩阵运算、特征值计算必须用）
template <typename T>
Vector<T> operator*(const Matrix<T>& mat, const Vector<T>& vec) {
    if (mat.cols() != vec.rows())
        throw std::invalid_argument("Matrix-Vector dimension mismatch");
    Vector<T> res(mat.rows());
    for (int i = 0; i < mat.rows(); ++i) {
        T sum = T(0);
        for (int j = 0; j < mat.cols(); ++j)
            sum += mat(i, j) * vec(j, 0);
        res(i, 0) = sum;
    }
    return res;
}

// 类型别名（兼容复数向量）
using ComplexVector = Vector<std::complex<double>>;

#endif