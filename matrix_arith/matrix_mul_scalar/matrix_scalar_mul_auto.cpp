#include "../../head_file/matrix.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <complex>
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
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (!(file >> mat(i, j))) return false;
    return true;
}

// 读取实数标量
bool loadRealScalar(ifstream& file, double& scalar) {
    return !!(file >> scalar);
}

// 读取复数矩阵
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

// 读取复数标量
bool loadComplexScalar(ifstream& file, Complex& scalar) {
    return !!(file >> scalar);
}

// 实数：矩阵数乘
void runRealScalarMul(ofstream& res, Matrix<double>& A, double s) {
    cout << "\nMatrix A:\n" << A;
    cout << "Scalar s = " << s << endl;
    res << "Matrix A:\n" << A << "\n";
    res << "Scalar s = " << s << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    Matrix<double> C = A * s;
    cout << "A * s =\n" << C;
    res << "A * s =\n" << C << "\n";
}

// 复数：矩阵数乘
void runComplexScalarMul(ofstream& res, Matrix<Complex>& A, Complex s) {
    cout << "\nMatrix A:\n" << A;
    cout << "Scalar s = " << s << endl;
    res << "Matrix A:\n" << A << "\n";
    res << "Scalar s = " << s << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    Matrix<Complex> C = A * s;
    cout << "A * s =\n" << C;
    res << "A * s =\n" << C << "\n";
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("matrix_scalar_mul_result.txt");
    cout << fixed << setprecision(2);
    resultFile << fixed << setprecision(2);

    cout << "===== AUTO Matrix Scalar Multiplication (REAL + COMPLEX) =====\n";
    cout << "Calculation: A * s\n\n";
    resultFile << "===== AUTO Matrix Scalar Multiplication =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix\n\n";
            resultFile << "Detected: COMPLEX matrix\n\n";

            Matrix<Complex> A;
            Complex s;
            if (file.is_open() && loadComplexMatrix(file, A) && loadComplexScalar(file, s)) {
                useFile = true;
                runComplexScalarMul(resultFile, A, s);
            }
        } else {
            cout << "Detected: REAL matrix\n\n";
            resultFile << "Detected: REAL matrix\n\n";

            Matrix<double> A;
            double s;
            if (file.is_open() && loadRealMatrix(file, A) && loadRealScalar(file, s)) {
                useFile = true;
                runRealScalarMul(resultFile, A, s);
            }
        }

        if (!useFile) {
            cout << "Failed to load data from file!\n";
            resultFile << "Load file failed, switch to manual input.\n\n";
        }
        file.close();
    }

    // -------------------- 手动输入 --------------------
    if (!useFile) {
        int choice;
        cout << "Choose matrix type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        if (choice == 1) {
            int r, c;
            cout << "\nEnter rows & cols for Matrix A: ";
            cin >> r >> c;
            Matrix<double> A(r, c);
            cout << "Enter elements of A: ";
            cin >> A;

            double s;
            cout << "Enter scalar value s: ";
            cin >> s;

            runRealScalarMul(resultFile, A, s);
        } else {
            int r, c;
            cout << "\nEnter rows & cols for Complex Matrix A: ";
            cin >> r >> c;
            Matrix<Complex> A(r, c);
            cout << "Enter complex elements: ";
            cin >> A;

            Complex s;
            cout << "Enter complex scalar s: ";
            cin >> s;

            runComplexScalarMul(resultFile, A, s);
        }
    }

    cout << "\nResult saved to matrix_scalar_mul_result.txt\n";
    resultFile.close();
    return 0;
}