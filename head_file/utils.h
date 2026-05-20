#ifndef UTILS_H
#define UTILS_H

#include "matrix.h"
#include <iostream>

template <typename T>
void inputMatrix(Matrix<T>& m) {
    std::cout << "Enter matrix elements (" << m.rows() << "x" << m.cols() << "):\n";
    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            std::cin >> m(i, j);
        }
    }
}

#endif