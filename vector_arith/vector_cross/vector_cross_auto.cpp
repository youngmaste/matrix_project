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

// 读取实数向量
bool loadRealVector(ifstream& file, Vector<double>& vec) {
    int dim;
    if (!(file >> dim)) return false;
    vec = Vector<double>(dim);
    for (int i = 0; i < dim; ++i) {
        if (!(file >> vec(i, 0))) return false;
    }
    return true;
}

// 读取复数向量（正确解析 (real,imag)）
bool loadComplexVector(ifstream& file, Vector<Complex>& vec) {
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
    return true;
}

// 实数向量叉积
void runRealCross(ofstream& res, Vector<double>& v1, Vector<double>& v2) {
    cout << "\nVector 1:\n" << v1;
    cout << "\nVector 2:\n" << v2 << "\n";
    res << "Vector 1:\n" << v1 << "\n";
    res << "Vector 2:\n" << v2 << "\n";

    cout << "----- Result -----\n";
    res << "----- Result -----\n";

    try {
        auto cross_val = v1.cross(v2);
        cout << "Cross Product:\n" << cross_val << "\n";
        res << "Cross Product:\n" << cross_val << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        res << "Error: " << e.what() << "\n";
    }
}

// 复数向量叉积
void runComplexCross(ofstream& res, Vector<Complex>& v1, Vector<Complex>& v2) {
    cout << "\nVector 1:\n" << v1;
    cout << "\nVector 2:\n" << v2 << "\n";
    res << "Vector 1:\n" << v1 << "\n";
    res << "Vector 2:\n" << v2 << "\n";

    cout << "----- Result -----\n";
    res << "----- Result -----\n";

    try {
        auto cross_val = v1.cross(v2);
        cout << "Cross Product:\n" << cross_val << "\n";
        res << "Cross Product:\n" << cross_val << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        res << "Error: " << e.what() << "\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("vector_cross_result.txt");
    cout << fixed << setprecision(4);
    resultFile << fixed << setprecision(4);

    cout << "===== AUTO 3D Vector Cross Product (REAL + COMPLEX) =====\n";
    cout << "Only support 3-dimensional vectors!\n\n";
    resultFile << "===== AUTO 3D Vector Cross Product (REAL + COMPLEX) =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComp = isComplexFile(filename);
        ifstream file(filename);

        if (isComp) {
            cout << "Detected: COMPLEX vectors\n";
            Vector<Complex> v1, v2;
            if (file.is_open() && loadComplexVector(file, v1) && loadComplexVector(file, v2)) {
                useFile = true;
                runComplexCross(resultFile, v1, v2);
            }
        } else {
            cout << "Detected: REAL vectors\n";
            Vector<double> v1, v2;
            if (file.is_open() && loadRealVector(file, v1) && loadRealVector(file, v2)) {
                useFile = true;
                runRealCross(resultFile, v1, v2);
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

        if (choice == 1) {
            Vector<double> v1(3), v2(3);
            cout << "Enter v1 (3 values): "; cin >> v1;
            cout << "Enter v2 (3 values): "; cin >> v2;
            runRealCross(resultFile, v1, v2);
        } else {
            Vector<Complex> v1(3), v2(3);
            cout << "Enter v1 (e.g. (1,0) (2,1) (3,2)): "; cin >> v1;
            cout << "Enter v2: "; cin >> v2;
            runComplexCross(resultFile, v1, v2);
        }
    }

    cout << "Result saved to vector_cross_result.txt\n";
    resultFile.close();
    return 0;
}