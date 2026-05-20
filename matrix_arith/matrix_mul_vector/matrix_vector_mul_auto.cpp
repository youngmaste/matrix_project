#include "../../head_file/matrix.h"
#include "../../head_file/vector.h"
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

// 读取实数向量
bool loadRealVector(ifstream& file, Vector<double>& vec) {
    int size;
    if (!(file >> size)) return false;
    vec = Vector<double>(size);
    for (int i = 0; i < size; ++i)
        if (!(file >> vec(i, 0))) return false;
    return true;
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

// 读取复数向量
bool loadComplexVector(ifstream& file, Vector<Complex>& vec) {
    int size;
    if (!(file >> size)) return false;
    vec = Vector<Complex>(size);
    char ch;
    double re, im;
    for (int i = 0; i < size; ++i) {
        file >> ch;
        file >> re;
        file >> ch;
        file >> im;
        file >> ch;
        vec(i, 0) = Complex(re, im);
    }
    return true;
}

// 实数：矩阵×向量
void runRealMV(ofstream& res, Matrix<double>& A, Vector<double>& v) {
    cout << "\nMatrix A:\n" << A;
    cout << "\nVector v:\n" << v;
    res << "Matrix A:\n" << A << "\n";
    res << "Vector v:\n" << v << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    try {
        Vector<double> ans = A * v;
        cout << "A * v =\n" << ans;
        res << "A * v =\n" << ans << "\n";
    } catch (...) {
        cout << "Error: Dimension mismatch! Matrix cols must equal vector size.\n";
        res << "Error: Dimension mismatch! Matrix cols must equal vector size.\n";
    }
}

// 复数：矩阵×向量
void runComplexMV(ofstream& res, Matrix<Complex>& A, Vector<Complex>& v) {
    cout << "\nMatrix A:\n" << A;
    cout << "\nVector v:\n" << v;
    res << "Matrix A:\n" << A << "\n";
    res << "Vector v:\n" << v << "\n";

    cout << "\n----- Result -----\n";
    res << "----- Result -----\n";

    try {
        Vector<Complex> ans = A * v;
        cout << "A * v =\n" << ans;
        res << "A * v =\n" << ans << "\n";
    } catch (...) {
        cout << "Error: Dimension mismatch! Matrix cols must equal vector size.\n";
        res << "Error: Dimension mismatch! Matrix cols must equal vector size.\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("matrix_vector_mul_result.txt");
    cout << fixed << setprecision(2);
    resultFile << fixed << setprecision(2);

    cout << "===== AUTO Matrix * Vector Multiplication (REAL + COMPLEX) =====\n";
    cout << "Calculation: A * v\n\n";
    resultFile << "===== AUTO Matrix * Vector Multiplication =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX matrix & vector\n\n";
            resultFile << "Detected: COMPLEX matrix & vector\n\n";

            Matrix<Complex> A;
            Vector<Complex> v;
            if (file.is_open() && loadComplexMatrix(file, A) && loadComplexVector(file, v)) {
                useFile = true;
                runComplexMV(resultFile, A, v);
            }
        } else {
            cout << "Detected: REAL matrix & vector\n\n";
            resultFile << "Detected: REAL matrix & vector\n\n";

            Matrix<double> A;
            Vector<double> v;
            if (file.is_open() && loadRealMatrix(file, A) && loadRealVector(file, v)) {
                useFile = true;
                runRealMV(resultFile, A, v);
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
        cout << "Choose type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        if (choice == 1) {
            int r, c, sz;
            cout << "\nEnter rows & cols for Matrix A: ";
            cin >> r >> c;
            Matrix<double> A(r, c);
            cout << "Enter elements of A: ";
            cin >> A;

            cout << "Enter size of Vector v: ";
            cin >> sz;
            Vector<double> v(sz);
            cout << "Enter elements of v: ";
            cin >> v;

            runRealMV(resultFile, A, v);
        } else {
            int r, c, sz;
            cout << "\nEnter rows & cols for Complex Matrix A: ";
            cin >> r >> c;
            Matrix<Complex> A(r, c);
            cout << "Enter complex elements: ";
            cin >> A;

            cout << "Enter size of Complex Vector v: ";
            cin >> sz;
            Vector<Complex> v(sz);
            cout << "Enter complex elements: ";
            cin >> v;

            runComplexMV(resultFile, A, v);
        }
    }

    cout << "\nResult saved to matrix_vector_mul_result.txt\n";
    resultFile.close();
    return 0;
}