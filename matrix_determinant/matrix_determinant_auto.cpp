#include "../head_file/matrix.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <complex>
#include <cmath>
#include <cstring>

using namespace std;
using Complex = complex<double>;

// 判断文件是否为复数格式
bool isComplexFile(const string& filename) {
    ifstream file(filename);
    string token;
    while (file >> token) {
        if (token.find('(') != string::npos)
            return true;
    }
    return false;
}

// 读取实数矩阵
bool loadRealMatrix(ifstream& file, Matrix<double>& mat) {
    int rows, cols;
    if (!(file >> rows >> cols)) return false;

    mat = Matrix<double>(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!(file >> mat(i, j))) return false;
        }
    }
    return true;
}

// 读取复数矩阵（解析 (real,imag)）
bool loadComplexMatrix(ifstream& file, Matrix<Complex>& mat) {
    int rows, cols;
    if (!(file >> rows >> cols)) return false;

    mat = Matrix<Complex>(rows, cols);
    char ch;
    double re, im;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> ch;
            file >> re;
            file >> ch;
            file >> im;
            file >> ch;
            mat(i, j) = Complex(re, im);
        }
    }
    return true;
}

// 实数行列式
void runRealDet(ofstream& res, Matrix<double>& A) {
    cout << "\nMatrix A:\n" << A;
    res << "Matrix A:\n" << A << "\n";

    cout << "\n----- Result -----\n";
    res << "\n----- Result -----\n";

    try {
        double det = A.determinant();
        cout << "Determinant(A) = " << det << "\n";
        res << "Determinant(A) = " << det << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        res << "Error: " << e.what() << "\n";
    }
}

// 复数行列式
void runComplexDet(ofstream& res, Matrix<Complex>& A) {
    cout << "\nMatrix A:\n" << A;
    res << "Matrix A:\n" << A << "\n";

    cout << "\n----- Result -----\n";
    res << "\n----- Result -----\n";

    try {
        Complex det = A.determinant();
        cout << "Determinant(A) = " << det << "\n";
        res << "Determinant(A) = " << det << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        res << "Error: " << e.what() << "\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("matrix_determinant_result.txt");
    cout << fixed << setprecision(4);
    resultFile << fixed << setprecision(4);

    cout << "===== AUTO Square Matrix Determinant (REAL + COMPLEX) =====\n";
    cout << "Algorithm: Gauss Elimination with Partial Pivoting (Numerical Stable)\n\n";
    resultFile << "===== AUTO Matrix Determinant =====\n";
    resultFile << "Algorithm: Numerical Stable Gauss Elimination\n\n";

    bool useFile = false;

    // 文件输入
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix\n\n";
            resultFile << "Detected: COMPLEX matrix\n\n";

            Matrix<Complex> A;
            if (file.is_open() && loadComplexMatrix(file, A)) {
                useFile = true;
                runComplexDet(resultFile, A);
            }
        } else {
            cout << "Detected: REAL matrix\n\n";
            resultFile << "Detected: REAL matrix\n\n";

            Matrix<double> A;
            if (file.is_open() && loadRealMatrix(file, A)) {
                useFile = true;
                runRealDet(resultFile, A);
            }
        }

        if (!useFile) {
            cout << "File load failed! Switch to manual input.\n\n";
            resultFile << "File load failed, switch to manual input.\n\n";
        }
        file.close();
    }

    // 手动输入
    if (!useFile) {
        int choice;
        cout << "Choose matrix type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        int n;
        cout << "Enter size n (n x n): ";
        cin >> n;

        if (choice == 1) {
            Matrix<double> A(n, n);
            cout << "Enter " << n*n << " elements: ";
            cin >> A;
            runRealDet(resultFile, A);
        } else {
            Matrix<Complex> A(n, n);
            cout << "Enter " << n*n << " complex elements: ";
            cin >> A;
            runComplexDet(resultFile, A);
        }
    }

    cout << "\nResult saved to matrix_determinant_result.txt\n";
    resultFile.close();
    return 0;
}