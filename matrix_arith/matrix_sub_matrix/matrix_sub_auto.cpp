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

// 实数矩阵减法 A - B
void runRealSub(ofstream& res, Matrix<double>& A, Matrix<double>& B) {
    cout << "\nMatrix A (Minuend):\n" << A;
    cout << "\nMatrix B (Subtrahend):\n" << B;
    res << "Matrix A (Minuend):\n" << A << "\n";
    res << "Matrix B (Subtrahend):\n" << B << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    try {
        Matrix<double> C = A - B;
        cout << "A - B =\n" << C;
        res << "A - B =\n" << C << "\n";
    } catch (...) {
        cout << " Error: Dimension mismatch!\n";
        res << "Error: Dimension mismatch!\n";
    }
}

// 复数矩阵减法 A - B
void runComplexSub(ofstream& res, Matrix<Complex>& A, Matrix<Complex>& B) {
    cout << "\nMatrix A (Minuend):\n" << A;
    cout << "\nMatrix B (Subtrahend):\n" << B;
    res << "Matrix A (Minuend):\n" << A << "\n";
    res << "Matrix B (Subtrahend):\n" << B << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    try {
        Matrix<Complex> C = A - B;
        cout << "A - B =\n" << C;
        res << "A - B =\n" << C << "\n";
    } catch (...) {
        cout << "Error: Dimension mismatch!\n";
        res << "Error: Dimension mismatch!\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("matrix_sub_result.txt");
    cout << fixed << setprecision(2);
    resultFile << fixed << setprecision(2);

    cout << "===== AUTO Matrix Subtraction (REAL + COMPLEX) =====\n";
    cout << "Calculation: A - B (A = Minuend, B = Subtrahend)\n\n";
    resultFile << "===== AUTO Matrix Subtraction =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix\n\n";
            resultFile << "Detected: COMPLEX matrix\n\n";

            Matrix<Complex> A, B;
            if (file.is_open() && loadComplexMatrix(file, A) && loadComplexMatrix(file, B)) {
                useFile = true;
                runComplexSub(resultFile, A, B);
            }
        } else {
            cout << "Detected: REAL matrix\n\n";
            resultFile << "Detected: REAL matrix\n\n";

            Matrix<double> A, B;
            if (file.is_open() && loadRealMatrix(file, A) && loadRealMatrix(file, B)) {
                useFile = true;
                runRealSub(resultFile, A, B);
            }
        }

        if (!useFile) {
            cout << "\n❌ Failed to load matrices from file!\n";
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
            // 实数矩阵输入
            int rA, cA, rB, cB;
            cout << "\nEnter rows & cols for Matrix A (Minuend): ";
            cin >> rA >> cA;
            Matrix<double> A(rA, cA);
            cout << "Enter elements of A: ";
            cin >> A;

            cout << "Enter rows & cols for Matrix B (Subtrahend): ";
            cin >> rB >> cB;
            Matrix<double> B(rB, cB);
            cout << "Enter elements of B: ";
            cin >> B;

            runRealSub(resultFile, A, B);
        } else {
            // 复数矩阵输入
            int rA, cA, rB, cB;
            cout << "\nEnter rows & cols for Complex Matrix A: ";
            cin >> rA >> cA;
            Matrix<Complex> A(rA, cA);
            cout << "Enter complex elements: ";
            cin >> A;

            cout << "Enter rows & cols for Complex Matrix B: ";
            cin >> rB >> cB;
            Matrix<Complex> B(rB, cB);
            cout << "Enter complex elements: ";
            cin >> B;

            runComplexSub(resultFile, A, B);
        }
    }

    cout << "\n Result saved to matrix_sub_result.txt\n";
    resultFile.close();
    return 0;
}