#include "../head_file/matrix.h"
#include "../head_file/vector.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <complex>
#include <cstring>

using namespace std;
using Complex = complex<double>;

// ------------------------------ 检查是否是复数格式 ------------------------------
bool isComplexFile(const string& filename) {
    ifstream file(filename);
    string word;
    while (file >> word) {
        if (word.find('(') != string::npos) {
            return true;
        }
    }
    return false;
}

// ------------------------------ 实数求解 ------------------------------
void solveReal(const string& filename, ofstream& resFile) {
    ifstream file(filename);
    int m, n;
    file >> m >> n;

    Matrix<double> A(m, n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            file >> A(i, j);

    Vector<double> b(m);
    for (int i = 0; i < m; i++)
        file >> b(i, 0);

    cout << "\nMatrix A:\n" << A;
    cout << "\nVector b:\n" << b;
    resFile << "Matrix A:\n" << A << "\n";
    resFile << "Vector b:\n" << b << "\n";

    auto x = A.solveLinear(b);
    cout << "\n----- Solution X -----\n";
    resFile << "\n----- Solution X -----\n";
    if (x.rows() == 0) {
        cout << "No solution\n";
        resFile << "No solution\n";
    } else {
        cout << x;
        resFile << x;
    }
}

// ------------------------------ 复数求解（正确读取） ------------------------------
void solveComplex(const string& filename, ofstream& resFile) {
    ifstream file(filename);
    int m, n;
    file >> m >> n;

    Matrix<Complex> A(m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            char ch[10];
            double re, im;
            file >> ch; // 读 (
            file >> re;
            file >> ch; // 读 ,
            file >> im;
            file >> ch; // 读 )
            A(i, j) = Complex(re, im);
        }
    }

    Vector<Complex> b(m);
    for (int i = 0; i < m; i++) {
        char ch[10];
        double re, im;
        file >> ch;
        file >> re;
        file >> ch;
        file >> im;
        file >> ch;
        b(i, 0) = Complex(re, im);
    }

    cout << "\nMatrix A:\n" << A;
    cout << "\nVector b:\n" << b;
    resFile << "Matrix A:\n" << A << "\n";
    resFile << "Vector b:\n" << b << "\n";

    auto x = A.solveLinear(b);
    cout << "\n----- Solution X -----\n";
    resFile << "\n----- Solution X -----\n";
    if (x.rows() == 0) {
        cout << "No solution\n";
        resFile << "No solution\n";
    } else {
        cout << x;
        resFile << x;
    }
}

// ------------------------------ 主函数 ------------------------------
int main(int argc, char* argv[]) {
    ofstream resFile("solution_result.txt");
    cout << fixed << setprecision(4);
    resFile << fixed << setprecision(4);

    cout << "===== AUTO Linear System Solver (REAL+COMPLEX) =====\n\n";
    resFile << "===== AUTO Linear System Solver (REAL+COMPLEX) =====\n\n";

    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix\n";
            solveComplex(filename, resFile);
        } else {
            cout << "Detected: REAL matrix\n";
            solveReal(filename, resFile);
        }
    } else {
        cout << "Usage: solver input.txt\n";
    }

    cout << "\nResult saved to solution_result.txt\n";
    resFile.close();
    return 0;
}