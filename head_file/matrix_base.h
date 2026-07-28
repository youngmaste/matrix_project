#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <complex>

template <typename T>
class MatrixBase {
public:
    // ── 尺寸查询 ──
    virtual int rows() const = 0;
    virtual int cols() const = 0;

    // ── 元素访问 ──
    virtual T& operator()(int i, int j) = 0;
    virtual const T& operator()(int i, int j) const = 0;

    // ── 显示 ──
    virtual void print() const = 0;

    // ── 基本属性（纯虚，任何矩阵都必须实现）──
    virtual int rank() const = 0;                // 秩
    virtual double norm_frobenius() const = 0;   // Frobenius 范数

    virtual ~MatrixBase() = default;
};

#endif
