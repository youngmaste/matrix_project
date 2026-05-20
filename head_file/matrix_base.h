#ifndef MATRIX_BASE_H
#define MATRIX_BASE_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <complex>

template <typename T>
class MatrixBase {
public:
    // 纯虚函数 = 抽象类
    virtual int rows() const = 0;
    virtual int cols() const = 0;
    virtual T& operator()(int i, int j) = 0;
    virtual const T& operator()(int i, int j) const = 0;
    virtual void print() const = 0;
    virtual bool loadFromFile(const std::string& path) = 0;
    virtual bool saveToFile(const std::string& path) const = 0;
    virtual ~MatrixBase() = default;

    // 多态运算声明
    virtual MatrixBase<T>* add(const MatrixBase<T>& other) const = 0;
    virtual MatrixBase<T>* sub(const MatrixBase<T>& other) const = 0;
    virtual MatrixBase<T>* mul(const MatrixBase<T>& other) const = 0;
};

#endif