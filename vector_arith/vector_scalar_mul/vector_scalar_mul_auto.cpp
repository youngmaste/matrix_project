#include "../../head_file/vector.h"
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

// 读取实数向量 + 实数标量
bool loadRealData(ifstream& file, Vector<double>& vec, double& scalar) {
    int dim;
    if (!(file >> dim)) return false;
    vec = Vector<double>(dim);
    for (int i = 0; i < dim; ++i) {
        if (!(file >> vec(i, 0))) return false;
    }
    if (!(file >> scalar)) return false;
    return true;
}

// 读取复数向量 + 复数标量
bool loadComplexData(ifstream& file, Vector<Complex>& vec, Complex& scalar) {
    int dim;
    if (!(file >> dim)) return false;
    vec = Vector<Complex>(dim);

    char ch;
    double re, im;
    for (int i = 0; i < dim; ++i) {
        file >> ch;
        file >> re;
        file >> ch;
        file >> im;
        file >> ch;
        vec(i, 0) = Complex(re, im);
    }

    file >> ch;
    file >> re;
    file >> ch;
    file >> im;
    file >> ch;
    scalar = Complex(re, im);
    return true;
}

// 实数运算
void runReal(ofstream& res, Vector<double>& v, double& s) {
    cout << "\nVector:\n" << v;
    cout << "Scalar = " << s << "\n\n";
    res << "Vector:\n" << v << "\n";
    res << "Scalar = " << s << "\n\n";

    cout << "----- Result -----\n";
    res << "----- Result -----\n";

    auto resVec = v * s;
    cout << "Scalar Multiplication:\n" << resVec << "\n";
    res << "Scalar Multiplication:\n" << resVec << "\n";
}

// 复数运算
void runComplex(ofstream& res, Vector<Complex>& v, Complex& s) {
    cout << "\nVector:\n" << v;
    cout << "Scalar = " << s << "\n\n";
    res << "Vector:\n" << v << "\n";
    res << "Scalar = " << s << "\n\n";

    cout << "----- Result -----\n";
    res << "----- Result -----\n";

    auto resVec = v * s;
    cout << "Scalar Multiplication:\n" << resVec << "\n";
    res << "Scalar Multiplication:\n" << resVec << "\n";
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("vector_scalar_mul_result.txt");
    cout << fixed << setprecision(4);
    resultFile << fixed << setprecision(4);

    cout << "===== AUTO Vector Scalar Multiplication (REAL + COMPLEX) =====\n\n";
    resultFile << "===== AUTO Vector Scalar Multiplication (REAL + COMPLEX) =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX vector & scalar\n\n";
            resultFile << "Detected: COMPLEX vector & scalar\n\n";

            Vector<Complex> v;
            Complex s;
            if (file.is_open() && loadComplexData(file, v, s)) {
                useFile = true;
                runComplex(resultFile, v, s);
            }
        } else {
            cout << "Detected: REAL vector & scalar\n\n";
            resultFile << "Detected: REAL vector & scalar\n\n";

            Vector<double> v;
            double s;
            if (file.is_open() && loadRealData(file, v, s)) {
                useFile = true;
                runReal(resultFile, v, s);
            }
        }

        if (!useFile) {
            cout << "File load failed! Switch to manual input.\n\n";
            resultFile << "File load failed! Switch to manual input.\n\n";
        }
        file.close();
    }

    // -------------------- 手动输入 --------------------
    if (!useFile) {
        int choice;
        cout << "Choose type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        int dim;
        cout << "Enter dimension: ";
        cin >> dim;

        if (choice == 1) {
            Vector<double> v(dim);
            double s;
            cout << "Enter vector: "; cin >> v;
            cout << "Enter scalar: "; cin >> s;
            runReal(resultFile, v, s);
        } else {
            Vector<Complex> v(dim);
            Complex s;
            cout << "Enter vector (e.g. (1,2) (3,4)): "; cin >> v;
            cout << "Enter scalar (e.g. (2,3)): "; cin >> s;
            runComplex(resultFile, v, s);
        }
    }

    cout << "Result saved to vector_scalar_mul_result.txt\n";
    resultFile.close();
    return 0;
}