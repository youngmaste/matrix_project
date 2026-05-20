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

// 判断文件是否包含复数格式 ( ... )
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

// 读取复数向量（正确解析格式）
bool loadComplexVector(ifstream& file, Vector<Complex>& vec) {
    int dim;
    if (!(file >> dim)) return false;
    vec = Vector<Complex>(dim);

    char ch;
    double re, im;
    for (int i = 0; i < dim; ++i) {
        file >> ch;   // 读 (
        file >> re;
        file >> ch;   // 读 ,
        file >> im;
        file >> ch;   // 读 )
        vec(i, 0) = Complex(re, im);
    }
    return true;
}

// 运行实数向量加法
void runRealAdd(ofstream& resultFile, Vector<double>& v1, Vector<double>& v2) {
    cout << "\nVector 1:\n" << v1;
    cout << "\nVector 2:\n" << v2 << "\n";
    resultFile << "Vector 1:\n" << v1 << "\n";
    resultFile << "Vector 2:\n" << v2 << "\n";

    cout << "----- Result -----\n";
    resultFile << "----- Result -----\n";

    try {
        Vector<double> sum_vec = v1 + v2;
        cout << "Vector Sum (v1 + v2):\n" << sum_vec << "\n";
        resultFile << "Vector Sum (v1 + v2):\n" << sum_vec << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        resultFile << "Error: " << e.what() << "\n";
    }
}

// 运行复数向量加法
void runComplexAdd(ofstream& resultFile, Vector<Complex>& v1, Vector<Complex>& v2) {
    cout << "\nVector 1:\n" << v1;
    cout << "\nVector 2:\n" << v2 << "\n";
    resultFile << "Vector 1:\n" << v1 << "\n";
    resultFile << "Vector 2:\n" << v2 << "\n";

    cout << "----- Result -----\n";
    resultFile << "----- Result -----\n";

    try {
        Vector<Complex> sum_vec = v1 + v2;
        cout << "Vector Sum (v1 + v2):\n" << sum_vec << "\n";
        resultFile << "Vector Sum (v1 + v2):\n" << sum_vec << "\n";
    } catch (const invalid_argument& e) {
        cout << "Error: " << e.what() << "\n";
        resultFile << "Error: " << e.what() << "\n";
    }
}

// ====================== 主函数 ======================
int main(int argc, char* argv[]) {
    ofstream resultFile("vector_add_result.txt");
    cout << fixed << setprecision(4);
    resultFile << fixed << setprecision(4);

    cout << "===== AUTO Vector Addition (REAL + COMPLEX) =====\n\n";
    resultFile << "===== AUTO Vector Addition (REAL + COMPLEX) =====\n\n";

    bool useFile = false;

    // -------------------- 文件输入模式 --------------------
    if (argc >= 2) {
        string filename = argv[1];
        bool isComplex = isComplexFile(filename);
        ifstream file(filename);

        if (isComplex) {
            cout << "Detected: COMPLEX vectors\n";
            Vector<Complex> v1, v2;
            if (file.is_open() && loadComplexVector(file, v1) && loadComplexVector(file, v2)) {
                useFile = true;
                runComplexAdd(resultFile, v1, v2);
            }
        } else {
            cout << "Detected: REAL vectors\n";
            Vector<double> v1, v2;
            if (file.is_open() && loadRealVector(file, v1) && loadRealVector(file, v2)) {
                useFile = true;
                runRealAdd(resultFile, v1, v2);
            }
        }

        if (!useFile) {
            cout << "File load failed! Switch to manual input.\n\n";
            resultFile << "File load failed! Switch to manual input.\n\n";
        }
        file.close();
    }

    // -------------------- 手动输入模式 --------------------
    if (!useFile) {
        int choice;
        cout << "Choose type:\n1 — Real\n2 — Complex\n>> ";
        cin >> choice;

        int dim;
        cout << "Enter dimension: ";
        cin >> dim;

        if (choice == 1) {
            Vector<double> v1(dim), v2(dim);
            cout << "Enter v1: "; cin >> v1;
            cout << "Enter v2: "; cin >> v2;
            runRealAdd(resultFile, v1, v2);
        } else {
            Vector<Complex> v1(dim), v2(dim);
            cout << "Enter v1 (e.g. (1,2) (3,4)): "; cin >> v1;
            cout << "Enter v2: "; cin >> v2;
            runComplexAdd(resultFile, v1, v2);
        }
    }

    cout << "Result saved to vector_add_result.txt\n";
    resultFile.close();
    return 0;
}