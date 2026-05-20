#include "../head_file/vector.h"
#include "../head_file/matrix.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <complex>
#include <cstring>

using namespace std;
using Complex = complex<double>;

// 判断文件是否是复数格式
bool isComplexFile(const string& filename) {
    ifstream file(filename);
    string token;
    while (file >> token) {
        if (token.find('(') != string::npos) {
            return true;
        }
    }
    return false;
}

// 读取实数向量
void loadRealVector(const string& filename, Vector<double>& v, bool& ok) {
    ifstream file(filename);
    int dim;
    file >> dim;
    v = Vector<double>(dim);
    for (int i = 0; i < dim; i++) {
        if (!(file >> v(i,0))) {
            ok = false;
            return;
        }
    }
    ok = true;
}

// 读取复数向量（正确解析 (real,imag)）
void loadComplexVector(const string& filename, Vector<Complex>& v, bool& ok) {
    ifstream file(filename);
    int dim;
    file >> dim;
    v = Vector<Complex>(dim);

    char ch;
    double re, im;
    for (int i = 0; i < dim; i++) {
        file >> ch;   // (
        file >> re;
        file >> ch;   // ,
        file >> im;
        file >> ch;   // )
        v(i,0) = Complex(re, im);
    }
    ok = true;
}

// 计算并输出实数向量范数
void runReal(Vector<double>& v, ofstream& resFile) {
    cout << "\nVector:\n" << v;
    resFile << "Vector:\n" << v << "\n";

    cout << "\n----- Vector Norms Result -----\n";
    resFile << "\n----- Vector Norms Result -----\n";

    double l1 = v.norm_l1();
    cout << "L1 Norm (p=1) = " << l1 << "\n";
    resFile << "L1 Norm (p=1) = " << l1 << "\n";

    double l2 = v.norm_l2();
    cout << "L2 Norm (p=2) = " << l2 << "\n";
    resFile << "L2 Norm (p=2) = " << l2 << "\n";

    double inf = v.norm_inf();
    cout << "Infinity Norm = " << inf << "\n";
    resFile << "Infinity Norm = " << inf << "\n";

    double p;
    cout << "\nEnter p value (p >= 1) for p-norm: ";
    cin >> p;
    resFile << "User input p = " << p << "\n";

    try {
        double pn = v.norm_p(p);
        cout << "p-Norm (p=" << p << ") = " << pn << "\n";
        resFile << "p-Norm (p=" << p << ") = " << pn << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        resFile << "Error: " << e.what() << "\n";
    }
}

// 计算并输出复数向量范数
void runComplex(Vector<Complex>& v, ofstream& resFile) {
    cout << "\nVector:\n" << v;
    resFile << "Vector:\n" << v << "\n";

    cout << "\n----- Vector Norms Result -----\n";
    resFile << "\n----- Vector Norms Result -----\n";

    double l1 = v.norm_l1();
    cout << "L1 Norm (p=1) = " << l1 << "\n";
    resFile << "L1 Norm (p=1) = " << l1 << "\n";

    double l2 = v.norm_l2();
    cout << "L2 Norm (p=2) = " << l2 << "\n";
    resFile << "L2 Norm (p=2) = " << l2 << "\n";

    double inf = v.norm_inf();
    cout << "Infinity Norm = " << inf << "\n";
    resFile << "Infinity Norm = " << inf << "\n";

    double p;
    cout << "\nEnter p value (p >= 1) for p-norm: ";
    cin >> p;
    resFile << "User input p = " << p << "\n";

    try {
        double pn = v.norm_p(p);
        cout << "p-Norm (p=" << p << ") = " << pn << "\n";
        resFile << "p-Norm (p=" << p << ") = " << pn << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        resFile << "Error: " << e.what() << "\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("vector_norm_result.txt");
    cout << fixed << setprecision(4);
    resultFile << fixed << setprecision(4);

    cout << "===== AUTO Vector Norms (REAL + COMPLEX) =====\n\n";
    resultFile << "===== AUTO Vector Norms (REAL + COMPLEX) =====\n\n";

    bool useFile = false;
    bool isComplex = false;

    if (argc >= 2) {
        string filename = argv[1];
        isComplex = isComplexFile(filename);

        if (isComplex) {
            cout << "Detected: COMPLEX vector\n";
            Vector<Complex> v;
            bool ok;
            loadComplexVector(filename, v, ok);
            if (ok) {
                useFile = true;
                runComplex(v, resultFile);
            }
        } else {
            cout << "Detected: REAL vector\n";
            Vector<double> v;
            bool ok;
            loadRealVector(filename, v, ok);
            if (ok) {
                useFile = true;
                runReal(v, resultFile);
            }
        }

        if (!useFile) {
            cout << "File load failed! Switch to manual input.\n\n";
            resultFile << "File load failed.\n\n";
        }
    }

    // 手动输入
    if (!useFile) {
        int dim;
        cout << "Choose vector type:\n";
        cout << "1 — Real\n";
        cout << "2 — Complex\n";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter dimension: ";
            cin >> dim;
            Vector<double> v(dim);
            cout << "Enter elements: ";
            cin >> v;
            runReal(v, resultFile);
        } else {
            cout << "Enter dimension: ";
            cin >> dim;
            Vector<Complex> v(dim);
            cout << "Enter complex elements (e.g. (1,2) (3,4)): ";
            cin >> v;
            runComplex(v, resultFile);
        }
    }

    cout << "\nResult saved to vector_norm_result.txt\n";
    resultFile.close();
    return 0;
}