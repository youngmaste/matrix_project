#include "../../head_file/matrix.h"
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
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (!(file >> mat(i, j))) return false;
    return true;
}

// 读取复数矩阵（正确解析 (real,imag)）
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

// 实数矩阵行和范数
void runRealRowNorm(ofstream& res, Matrix<double>& A) {
    cout << "\nMatrix A:\n" << A;
    res << "Matrix A:\n" << A << "\n";

    cout << "\n----- Result -----\n";
    res << "\n----- Result -----\n";

    double row_norm = A.norm_row();
    cout << "Matrix Row Sum Norm = " << row_norm << "\n";
    res << "Row Sum Norm = " << row_norm << "\n";
}

// 复数矩阵行和范数
void runComplexRowNorm(ofstream& res, Matrix<Complex>& A) {
    cout << "\nMatrix A:\n" << A;
    res << "Matrix A:\n" << A << "\n";

    cout << "\n----- Result -----\n";
    res << "\n----- Result -----\n";

    double row_norm = A.norm_row();
    cout << "Complex Matrix Row Sum Norm = " << row_norm << "\n";
    res << "Row Sum Norm = " << row_norm << "\n";
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resFile("matrix_row_norm_result.txt");
    cout << fixed << setprecision(4);
    resFile << fixed << setprecision(4);

    cout << "===== AUTO Matrix Row Sum Norm (REAL + COMPLEX) =====\n\n";
    resFile << "===== AUTO Matrix Row Sum Norm =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix\n\n";
            resFile << "Detected: COMPLEX matrix\n\n";

            Matrix<Complex> A;
            if (file.is_open() && loadComplexMatrix(file, A)) {
                useFile = true;
                runComplexRowNorm(resFile, A);
            }
        } else {
            cout << "Detected: REAL matrix\n\n";
            resFile << "Detected: REAL matrix\n\n";

            Matrix<double> A;
            if (file.is_open() && loadRealMatrix(file, A)) {
                useFile = true;
                runRealRowNorm(resFile, A);
            }
        }

        if (!useFile) {
            cout << "File load failed! Switch to manual input.\n\n";
            resFile << "File load failed, switch to manual input.\n\n";
        }
        file.close();
    }

    // -------------------- 手动输入 --------------------
    if (!useFile) {
        int choice;
        cout << "Choose matrix type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        int r, c;
        cout << "Input rows cols: ";
        cin >> r >> c;

        if (choice == 1) {
            Matrix<double> A(r, c);
            cout << "Input matrix elements:\n";
            cin >> A;
            runRealRowNorm(resFile, A);
        } else {
            Matrix<Complex> A(r, c);
            cout << "Input complex elements:\n";
            cin >> A;
            runComplexRowNorm(resFile, A);
        }
    }

    resFile.close();
    return 0;
}