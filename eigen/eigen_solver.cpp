#include "../head_file/matrix.h"
#include "../head_file/vector.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <complex>
#include <vector>

using namespace std;
using Complex = complex<double>;
const double eps = 1e-8;

bool isComplexFile(const string& filename) {
    ifstream file(filename);
    string word;
    while (file >> word) {
        if (word.find('(') != string::npos) return true;
    }
    return false;
}

void solveRealMatrix(const string& filename, ofstream& out) {
    ifstream file(filename);
    int n;
    file >> n >> n;

    Matrix<double> A(n, n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> A(i, j);

    cout << " Real Matrix A:\n" << A << endl;
    out << "Real Matrix A:\n" << A << "\n\n";

    auto eig = A.eigenvalues();
    bool diag = A.isDiagonalizable();

    cout << "===== Eigenvalues & Eigenvectors =====\n" << endl;
    out << "===== Eigenvalues & Eigenvectors =====\n\n";

    for (size_t i=0;i<eig.size();i++){
        Complex lam = eig[i];
        bool dup = false;
        for(size_t j=0;j<i;j++) if(abs(eig[j]-lam)<eps) {dup=true;break;}
        if(dup) continue;

        auto vec = A.eigenVector(lam);
        int cnt=0;
        for(auto x:eig) if(abs(x-lam)<eps) cnt++;
        Matrix<Complex> M = A.cast<Complex>();
        Matrix<Complex> L(n,n);
        for(int k=0;k<n;k++) L(k,k)=lam;
        int g = n - (M-L).rank();

        // 移除特殊符号λ，修复中文乱码
        cout << "Eigenvalue = " << lam << endl;
        cout << "Algebraic Multiplicity: " << cnt << endl;
        cout << "Geometric Multiplicity: " << g << endl;
        cout << "Eigenvector:\n" << vec << endl;

        out << "Eigenvalue = " << lam << "\n";
        out << "Algebraic Multiplicity: " << cnt << "\n";
        out << "Geometric Multiplicity: " << g << "\n";
        out << "Eigenvector:\n" << vec << "\n\n";
    }

    // 移除乱码符号，纯英文显示
    cout << "Diagonalizable? : " << (diag ? "Yes" : "No") << endl;
    out << "Diagonalizable? : " << (diag ? "Yes" : "No") << "\n";
}

void solveComplexMatrix(const string& filename, ofstream& out) {
    ifstream file(filename);
    int n;
    file >> n >> n;

    Matrix<Complex> A(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char ch;
            double re, im;
            file >> ch >> re >> ch >> im >> ch;
            A(i, j) = Complex(re, im);
        }
    }

    cout << " Complex Matrix A:\n" << A << endl;
    out << "Complex Matrix A:\n" << A << "\n\n";

    auto eig = A.eigenvalues();
    bool diag = A.isDiagonalizable();

    cout << "===== Eigenvalues & Eigenvectors =====\n" << endl;
    out << "===== Eigenvalues & Eigenvectors =====\n\n";

    for (size_t i=0;i<eig.size();i++){
        Complex lam = eig[i];
        bool dup = false;
        for(size_t j=0;j<i;j++) if(abs(eig[j]-lam)<eps) {dup=true;break;}
        if(dup) continue;

        auto vec = A.eigenVector(lam);
        int cnt=0;
        for(auto x:eig) if(abs(x-lam)<eps) cnt++;
        Matrix<Complex> L(n,n);
        for(int k=0;k<n;k++) L(k,k)=lam;
        int g = n - (A-L).rank();

        // 移除特殊符号λ
        cout << "Eigenvalue = " << lam << endl;
        cout << "Algebraic Multiplicity: " << cnt << endl;
        cout << "Geometric Multiplicity: " << g << endl;
        cout << "Eigenvector:\n" << vec << endl;

        out << "Eigenvalue = " << lam << "\n";
        out << "Algebraic Multiplicity: " << cnt << "\n";
        out << "Geometric Multiplicity: " << g << "\n";
        out << "Eigenvector:\n" << vec << "\n\n";
    }

    cout << "Diagonalizable? : " << (diag ? "Yes" : "No") << endl;
    out << "Diagonalizable? : " << (diag ? "Yes" : "No") << "\n";
}

int main(int argc, char* argv[]) {
    cout << fixed << setprecision(4);
    ofstream outFile("eigen_result.txt");
    outFile << fixed << setprecision(4);

    cout << "===== Eigenvalue & Eigenvector Solver (Real & Complex) =====\n" << endl;
    outFile << "===== Eigenvalue & Eigenvector Solver =====\n\n";

    if (argc < 2) {
        cout << "Usage: .\\eigen_solver.exe input_file.txt" << endl;
        return 0;
    }

    string filename = argv[1];
    if (isComplexFile(filename)) {
        cout << " Detected: Complex Matrix\n" << endl;
        outFile << "Matrix Type: Complex\n\n";
        solveComplexMatrix(filename, outFile);
    } else {
        cout << " Detected: Real Matrix\n" << endl;
        outFile << "Matrix Type: Real\n\n";
        solveRealMatrix(filename, outFile);
    }

    cout << "\n Results saved to eigen_result.txt" << endl;
    outFile.close();
    return 0;
}