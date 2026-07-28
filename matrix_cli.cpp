// =====================================================================
//  matrix_cli.cpp -- Unified Matrix Linear Algebra CLI
//  合并所有功能于单一可执行文件,消除重复代码
// =====================================================================

#include "head_file/matrix.h"
#include "head_file/vector.h"
#include "head_file/utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <complex>
#include <cmath>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;
using Complex = complex<double>;
const double EPS = 1e-10;

// CLI direct-mode: set by command-line args, consumed by promptFilename()
string g_cli_file;

// ===================================================================
//  SECTION 0: Shared Utility Functions (defined ONCE, used everywhere)
int chooseType() {
    int choice;
    cerr << "  1 -- Real\n  2 -- Complex\n  >> ";
    cin >> choice;
    return choice;
}

// --- Prompt: file path or interactive ---
string promptFilename() {
    if (!g_cli_file.empty()) {
        string s = g_cli_file;
        g_cli_file.clear();
        // CLI mode: use the file path without asking
        return s;
    }
    cerr << "  Enter file path (or 0 for manual input): ";
    string s;
    cin >> s;
    return s;
}

void separator() { cerr << "\n" << string(48, '=') << "\n"; }

// ===================================================================
//  SECTION 1: Matrix Arithmetic
// ===================================================================
//判断是文件输入还是命令行输入数据，以及是实数还是复数，以及合法性检查
void matrixAdd() {
    separator();
    cerr << "  1.1  Matrix Addition (A + B)\n";
    
    cout << "\n[1.1] Matrix Addition\n";
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A, B;
            if (loadComplexMatrix(f, A) && loadComplexMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A + B =\n" << (A + B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            } else { cerr << "File load failed!\n"; }
        } else {
            ifstream f(file);
            Matrix<double> A, B;
            if (loadRealMatrix(f, A) && loadRealMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A + B =\n" << (A + B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            } else { cerr << "File load failed!\n"; }
        }
    } else {
        if (chooseType() == 2) {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<Complex> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<Complex> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            cout << "A =\n" << A << "\nB =\n" << B;
            try { cout << "A + B =\n" << (A + B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<double> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<double> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            cout << "A =\n" << A << "\nB =\n" << B;
            try { cout << "A + B =\n" << (A + B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void matrixSub() {
    separator();
    cerr << "  1.2  Matrix Subtraction (A - B)\n";
    
    cout << "\n[1.2] Matrix Subtraction\n";
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A, B;
            if (loadComplexMatrix(f, A) && loadComplexMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A - B =\n" << (A - B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A, B;
            if (loadRealMatrix(f, A) && loadRealMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A - B =\n" << (A - B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<Complex> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<Complex> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            try { cout << "A - B =\n" << (A - B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<double> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<double> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            try { cout << "A - B =\n" << (A - B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void matrixMul() {
    separator();
    cerr << "  1.3  Matrix Multiplication (A * B)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A, B;
            if (loadComplexMatrix(f, A) && loadComplexMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A * B =\n" << (A * B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A, B;
            if (loadRealMatrix(f, A) && loadRealMatrix(f, B)) {
                cout << "A =\n" << A << "\nB =\n" << B;
                try { cout << "A * B =\n" << (A * B); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<Complex> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<Complex> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            try { cout << "A * B =\n" << (A * B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int ra, ca, rb, cb;
            cerr << "  Rows Cols for A: "; cin >> ra >> ca;
            Matrix<double> A(ra, ca); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cerr << "  Rows Cols for B: "; cin >> rb >> cb;
            Matrix<double> B(rb, cb); cerr << "  Enter B: "; if (!readMatrixFromLine(B)) return;
            try { cout << "A * B =\n" << (A * B); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void matrixScalarOp() {
    separator();
    cerr << "  1.4  Matrix Scalar Multiplication & Division\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A; Complex s;
            if (loadComplexMatrix(f, A)) {
                f >> s;
                cout << "A =\n" << A << "\nScalar = " << s << "\n";
                cout << "A * s =\n" << (A * s);
                try { cout << "A / s =\n" << (A / s); }
                catch (const invalid_argument& e) { cout << "Division error: " << e.what() << "\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A; double s;
            if (loadRealMatrix(f, A)) {
                f >> s;
                cout << "A =\n" << A << "\nScalar = " << s << "\n";
                cout << "A * s =\n" << (A * s);
                try { cout << "A / s =\n" << (A / s); }
                catch (const invalid_argument& e) { cout << "Division error: " << e.what() << "\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            Complex s; cout << "  Enter scalar: "; cin >> s;
            cout << "A * s =\n" << (A * s);
            try { cout << "A / s =\n" << (A / s); }
            catch (const invalid_argument& e) { cout << e.what() << "\n"; }
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            double s; cout << "  Enter scalar: "; cin >> s;
            cout << "A * s =\n" << (A * s);
            try { cout << "A / s =\n" << (A / s); }
            catch (const invalid_argument& e) { cout << e.what() << "\n"; }
        }
    }
}

void matrixVectorMul() {
    separator();
    cerr << "  1.5  Matrix * Vector (A * v)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) {
                // Continue reading vector from same stream (don't reopen!)
                int dim; f >> dim;
                Vector<Complex> v(dim);
                for (int i = 0; i < dim; ++i) {
                    char ch; double re, im;
                    f >> ch >> re >> ch >> im >> ch;
                    v(i, 0) = Complex(re, im);
                }
                cout << "A =\n" << A << "\nv =\n" << v;
                try { cout << "A * v =\n" << (A * v); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A;
            if (loadRealMatrix(f, A)) {
                // Continue reading vector from same stream
                int dim; f >> dim;
                Vector<double> v(dim);
                for (int i = 0; i < dim; ++i) f >> v(i, 0);
                cout << "A =\n" << A << "\nv =\n" << v;
                try { cout << "A * v =\n" << (A * v); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols for A: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A (" << r*c << " elts): "; if (!readMatrixFromLine(A)) return;
            int vd; cerr << "  Vector dimension: "; cin >> vd;
            if (vd != c) { cin.ignore(10000, '\n'); cout << "Dimension mismatch! Vector dim must be " << c << "\n"; return; }
            Vector<Complex> v(vd); cerr << "  Enter v: "; if (!readComplexVectorFromLine(vd, v)) return;
            try { cout << "A * v =\n" << (A * v); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int r, c; cerr << "  Rows Cols for A: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A (" << r*c << " elts): "; if (!readMatrixFromLine(A)) return;
            int vd; cerr << "  Vector dimension: "; cin >> vd;
            if (vd != c) { cin.ignore(10000, '\n'); cout << "Dimension mismatch! Vector dim must be " << c << "\n"; return; }
            Vector<double> v(vd); cerr << "  Enter v: "; if (!readVectorFromLine(vd, v)) return;
            try { cout << "A * v =\n" << (A * v); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

// ===================================================================
//  SECTION 2: Vector Arithmetic
// ===================================================================

void vectorAdd() {
    separator();
    cerr << "  2.1  Vector Addition (v1 + v2)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v1 = readComplexVec(f);
            auto v2 = readComplexVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 + v2 =\n" << (v1 + v2); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            auto v1 = readRealVec(f);
            auto v2 = readRealVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 + v2 =\n" << (v1 + v2); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<Complex> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 + v2 =\n" << (v1 + v2); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<double> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 + v2 =\n" << (v1 + v2); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void vectorSub() {
    separator();
    cerr << "  2.2  Vector Subtraction (v1 - v2)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v1 = readComplexVec(f);
            auto v2 = readComplexVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 - v2 =\n" << (v1 - v2); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            auto v1 = readRealVec(f);
            auto v2 = readRealVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 - v2 =\n" << (v1 - v2); }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<Complex> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 - v2 =\n" << (v1 - v2); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<double> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 - v2 =\n" << (v1 - v2); }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void vectorDot() {
    separator();
    cerr << "  2.3  Vector Dot Product (v1 - v2)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v1 = readComplexVec(f);
            auto v2 = readComplexVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 - v2 = " << v1.dot(v2) << "\n"; }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        } else {
            ifstream f(file);
            auto v1 = readRealVec(f);
            auto v2 = readRealVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 - v2 = " << v1.dot(v2) << "\n"; }
                catch (...) { cout << "Dimension mismatch!\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<Complex> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 - v2 = " << v1.dot(v2) << "\n"; }
            catch (...) { cout << "Dimension mismatch!\n"; }
        } else {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<double> v1(d), v2(d);
            cerr << "  Enter v1: "; if (!readVectorFromLine(d, v1)) return;
            cerr << "  Enter v2: "; if (!readVectorFromLine(d, v2)) return;
            try { cout << "v1 - v2 = " << v1.dot(v2) << "\n"; }
            catch (...) { cout << "Dimension mismatch!\n"; }
        }
    }
}

void vectorCross() {
    separator();
    cerr << "  2.4  3D Vector Cross Product (v1 x v2)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v1 = readComplexVec(f);
            auto v2 = readComplexVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 x v2 =\n" << v1.cross(v2); }
                catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
            }
        } else {
            ifstream f(file);
            auto v1 = readRealVec(f);
            auto v2 = readRealVec(f);
             { cout << "v1 =\n" << v1 << "\nv2 =\n" << v2;
                try { cout << "v1 x v2 =\n" << v1.cross(v2); }
                catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            Vector<Complex> v1(3), v2(3);
            cerr << "  Enter v1 (3 complex): "; if (!readComplexVectorFromLine(3, v1)) return;
            cerr << "  Enter v2 (3 complex): "; if (!readComplexVectorFromLine(3, v2)) return;
            try { cout << "v1 x v2 =\n" << v1.cross(v2); }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            Vector<double> v1(3), v2(3);
            cerr << "  Enter v1 (3 elts): "; if (!readVectorFromLine(3, v1)) return;
            cerr << "  Enter v2 (3 elts): "; if (!readVectorFromLine(3, v2)) return;
            try { cout << "v1 x v2 =\n" << v1.cross(v2); }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        }
    }
}

void vectorScalarMul() {
    separator();
    cerr << "  2.5  Vector Scalar Multiplication\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v = readComplexVec(f);
            Complex s; f >> s;
            cout << "v =\n" << v << "\nScalar = " << s << "\n";
            cout << "v * s =\n" << (v * s);
        } else {
            ifstream f(file);
            auto v = readRealVec(f);
            double s; f >> s;
            cout << "v =\n" << v << "\nScalar = " << s << "\n";
            cout << "v * s =\n" << (v * s);
        }
    } else {
        if (chooseType() == 2) {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<Complex> v(d); cerr << "  Enter v: "; if (!readComplexVectorFromLine(d, v)) return;
            Complex s; cout << "  Enter scalar: "; cin >> s;
            cout << "v * s =\n" << (v * s);
        } else {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<double> v(d); cerr << "  Enter v: "; if (!readVectorFromLine(d, v)) return;
            double s; cout << "  Enter scalar: "; cin >> s;
            cout << "v * s =\n" << (v * s);
        }
    }
}

void vectorNorms() {
    separator();
    cerr << "  2.6  Vector Norms (L1, L2, Lp, Linf)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            auto v = readComplexVec(f);
            cout << "v =\n" << v;
            cout << "L1   = " << v.norm_l1() << "\n";
            cout << "L2   = " << v.norm_l2() << "\n";
            cout << "Linf   = " << v.norm_inf() << "\n";
            double p; cout << "Enter p (>=1) for p-norm: "; cin >> p;
            try { cout << "L" << p << "   = " << v.norm_p(p) << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            ifstream f(file);
            auto v = readRealVec(f);
            cout << "v =\n" << v;
            cout << "L1   = " << v.norm_l1() << "\n";
            cout << "L2   = " << v.norm_l2() << "\n";
            cout << "Linf   = " << v.norm_inf() << "\n";
            double p; cout << "Enter p (>=1) for p-norm: "; cin >> p;
            try { cout << "L" << p << "   = " << v.norm_p(p) << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        }
    } else {
        if (chooseType() == 2) {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<Complex> v(d); cerr << "  Enter v: "; if (!readComplexVectorFromLine(d, v)) return;
            cout << "L1   = " << v.norm_l1() << "\n";
            cout << "L2   = " << v.norm_l2() << "\n";
            cout << "Linf   = " << v.norm_inf() << "\n";
            double p; cout << "Enter p (>=1): "; cin >> p;
            try { cout << "L" << p << "   = " << v.norm_p(p) << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            int d; cerr << "  Dimension: "; cin >> d;
            Vector<double> v(d); cerr << "  Enter v: "; if (!readVectorFromLine(d, v)) return;
            cout << "L1   = " << v.norm_l1() << "\n";
            cout << "L2   = " << v.norm_l2() << "\n";
            cout << "Linf   = " << v.norm_inf() << "\n";
            double p; cout << "Enter p (>=1): "; cin >> p;
            try { cout << "L" << p << "   = " << v.norm_p(p) << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        }
    }
}

// ===================================================================
//  SECTION 3: Core Matrix Operations
// ===================================================================

void matrixDeterminant() {
    separator();
    cerr << "  3.1  Matrix Determinant\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) {
                cout << "A =\n" << A;
                try { cout << "det(A) = " << A.determinant() << "\n"; }
                catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A;
            if (loadRealMatrix(f, A)) {
                cout << "A =\n" << A;
                try { cout << "det(A) = " << A.determinant() << "\n"; }
                catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int n; cerr << "  Size n (square): "; cin >> n;
            Matrix<Complex> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try { cout << "det(A) = " << A.determinant() << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            int n; cerr << "  Size n (square): "; cin >> n;
            Matrix<double> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try { cout << "det(A) = " << A.determinant() << "\n"; }
            catch (const invalid_argument& e) { cout << "Error: " << e.what() << "\n"; }
        }
    }
}

void matrixAdjugateInverse() {
    separator();
    cerr << "  3.2  Adjugate Matrix & Inverse Matrix\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) {
                cout << "A =\n" << A;
                try {
                    auto det = A.determinant();
                    cout << "det(A) = " << det << "\n";
                    cout << "Adjugate:\n" << A.adjugate();
                    if (abs(det) > EPS) cout << "Inverse:\n" << A.inverse();
                    else cout << "Singular -> no inverse!\n";
                } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
            }
        } else {
            ifstream f(file);
            Matrix<double> A;
            if (loadRealMatrix(f, A)) {
                cout << "A =\n" << A;
                try {
                    auto det = A.determinant();
                    cout << "det(A) = " << det << "\n";
                    cout << "Adjugate:\n" << A.adjugate();
                    if (fabs(det) > EPS) cout << "Inverse:\n" << A.inverse();
                    else cout << "Singular -> no inverse!\n";
                } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int n; cerr << "  Size n: "; cin >> n;
            Matrix<Complex> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try {
                auto det = A.determinant();
                cout << "det(A) = " << det << "\nAdjugate:\n" << A.adjugate();
                if (abs(det) > EPS) cout << "Inverse:\n" << A.inverse();
                else cout << "Singular -> no inverse!\n";
            } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            int n; cerr << "  Size n: "; cin >> n;
            Matrix<double> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try {
                auto det = A.determinant();
                cout << "det(A) = " << det << "\nAdjugate:\n" << A.adjugate();
                if (fabs(det) > EPS) cout << "Inverse:\n" << A.inverse();
                else cout << "Singular -> no inverse!\n";
            } catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
        }
    }
}

void matrixTranspose() {
    separator();
    cerr << "  3.3  Matrix Transpose\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A << "\nA^T =\n" << A.transpose(); }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A << "\nA^T =\n" << A.transpose(); }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "A^T =\n" << A.transpose();
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "A^T =\n" << A.transpose();
        }
    }
}

void matrixRankRREF() {
    separator();
    cerr << "  3.4  Matrix Rank & RREF\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) {
                cout << "A =\n" << A;
                cout << "Rank = " << A.rank() << "\n" << A.rref();
            }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) {
                cout << "A =\n" << A;
                cout << "Rank = " << A.rank() << "\n" << A.rref();
            }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "Rank = " << A.rank() << "\n" << A.rref();
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "Rank = " << A.rank() << "\n" << A.rref();
        }
    }
}

// ===================================================================
//  SECTION 4: Matrix Norms & Condition Number
// ===================================================================

void matrixFrobeniusNorm() {
    separator();
    cerr << "  4.1  Frobenius Norm\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A << "||A||_F = " << A.norm_frobenius() << "\n"; }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A << "||A||_F = " << A.norm_frobenius() << "\n"; }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_F = " << A.norm_frobenius() << "\n";
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_F = " << A.norm_frobenius() << "\n";
        }
    }
}

void matrixRowNorm() {
    separator();
    cerr << "  4.2  Row Sum Norm (inf-Norm)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A << "||A||_inf = " << A.norm_row() << "\n"; }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A << "||A||_inf = " << A.norm_row() << "\n"; }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_inf = " << A.norm_row() << "\n";
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_inf = " << A.norm_row() << "\n";
        }
    }
}

void matrixColNorm() {
    separator();
    cerr << "  4.3  Column Sum Norm (1-Norm)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A << "||A||_1 = " << A.norm_col() << "\n"; }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A << "||A||_1 = " << A.norm_col() << "\n"; }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_1 = " << A.norm_col() << "\n";
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            cout << "||A||_1 = " << A.norm_col() << "\n";
        }
    }
}

void matrixSpectralNorm() {
    separator();
    cerr << "  4.4  Spectral Norm (sigma_max via Power Iteration)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A;
                try { cout << "||A||_2 = " << A.norm_spectral() << "\n"; }
                catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
            }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A;
                try { cout << "||A||_2 = " << A.norm_spectral() << "\n"; }
                catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
            }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try { cout << "||A||_2 = " << A.norm_spectral() << "\n"; }
            catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            try { cout << "||A||_2 = " << A.norm_spectral() << "\n"; }
            catch (const exception& e) { cout << "Error: " << e.what() << "\n"; }
        }
    }
}

void matrixCondition() {
    separator();
    cerr << "  4.5  Condition Number  cond(A) = ||A|| - ||A-1||\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) { cout << "A =\n" << A;
                double c = A.conditionNumber();
                if (c == HUGE_VAL) cout << "cond(A) = infinity\n";
                else cout << "cond(A) = " << c << "\n";
            }
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) { cout << "A =\n" << A;
                double c = A.conditionNumber();
                if (c == HUGE_VAL) cout << "cond(A) = infinity\n";
                else cout << "cond(A) = " << c << "\n";
            }
        }
    } else {
        if (chooseType() == 2) {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<Complex> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            double cn = A.conditionNumber();
            if (cn == HUGE_VAL) cout << "cond(A) = infinity\n";
            else cout << "cond(A) = " << cn << "\n";
        } else {
            int r, c; cerr << "  Rows Cols: "; cin >> r >> c;
            Matrix<double> A(r, c); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            double cn = A.conditionNumber();
            if (cn == HUGE_VAL) cout << "cond(A) = infinity\n";
            else cout << "cond(A) = " << cn << "\n";
        }
    }
}

// ===================================================================
//  SECTION 5: Linear System Solver
// ===================================================================

void linearSolve() {
    separator();
    cerr << "  5.1  Linear System Solver (A x = b)\n";
    
    string file = promptFilename();

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file);
            int m, n; f >> m >> n;
            Matrix<Complex> A(m, n);
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j) {
                    char ch; double re, im;
                    f >> ch >> re >> ch >> im >> ch;
                    A(i, j) = Complex(re, im);
                }
            Vector<Complex> b(m);
            for (int i = 0; i < m; ++i) {
                char ch; double re, im;
                f >> ch >> re >> ch >> im >> ch;
                b(i, 0) = Complex(re, im);
            }
            cout << "A =\n" << A << "\nb =\n" << b;
            auto x = A.solveLinear(b);
            if (x.rows() == 0) cout << "No solution!\n";
            else cout << "x =\n" << x;
        } else {
            ifstream f(file);
            int m, n; f >> m >> n;
            Matrix<double> A(m, n);
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < n; ++j) f >> A(i, j);
            Vector<double> b(m);
            for (int i = 0; i < m; ++i) f >> b(i, 0);
            cout << "A =\n" << A << "\nb =\n" << b;
            auto x = A.solveLinear(b);
            if (x.rows() == 0) cout << "No solution!\n";
            else cout << "x =\n" << x;
        }
    } else {
        if (chooseType() == 2) {
            int m, n; cerr << "  Rows Cols for A: "; cin >> m >> n;
            Matrix<Complex> A(m, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            Vector<Complex> b(m); cout << "  Enter b (" << m << " elts): "; cin >> b;
            auto x = A.solveLinear(b);
            if (x.rows() == 0) cout << "No solution!\n";
            else cout << "x =\n" << x;
        } else {
            int m, n; cerr << "  Rows Cols for A: "; cin >> m >> n;
            Matrix<double> A(m, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            Vector<double> b(m); cout << "  Enter b (" << m << " elts): "; cin >> b;
            auto x = A.solveLinear(b);
            if (x.rows() == 0) cout << "No solution!\n";
            else cout << "x =\n" << x;
        }
    }
}

// ===================================================================
//  SECTION 6: Eigenvalues & Eigenvectors
// ===================================================================

void eigenSolve() {
    separator();
    cerr << "  6.1  Eigenvalues & Eigenvectors\n";
    
    string file = promptFilename();

    auto processEigen = [](auto& A) {
        using T = typename decay<decltype(A(0,0))>::type;
        cout << "A =\n" << A;
        auto eig = A.eigenvalues();
        bool diag = A.isDiagonalizable();
        cout << "\n--- Eigenvalues & Eigenvectors ---\n\n";

        for (size_t i = 0; i < eig.size(); ++i) {
            Complex lam = eig[i];
            bool dup = false;
            for (size_t j = 0; j < i; ++j) if (abs(eig[j] - lam) < EPS) { dup = true; break; }
            if (dup) continue;

            auto vec = A.eigenVector(lam);
            int cnt = 0;
            for (auto x : eig) if (abs(x - lam) < EPS) cnt++;

            int n = A.rows();
            Matrix<Complex> Ac = A.template cast<Complex>();
            Matrix<Complex> L(n, n);
            for (int k = 0; k < n; ++k) L(k, k) = lam;
            int g = n - (Ac - L).rank();

            cout << "lam = " << lam << "\n";
            cout << "  Algebraic Mult:  " << cnt << "\n";
            cout << "  Geometric Mult:  " << g << "\n";
            cout << "  Eigenvector:\n" << vec << "\n";
        }
        cout << "Diagonalizable?  " << (diag ? "YES" : "NO") << "\n";
    };

    if (file != "0") {
        if (isComplexFile(file)) {
            ifstream f(file); Matrix<Complex> A;
            if (loadComplexMatrix(f, A)) processEigen(A);
        } else {
            ifstream f(file); Matrix<double> A;
            if (loadRealMatrix(f, A)) processEigen(A);
        }
    } else {
        if (chooseType() == 2) {
            int n; cerr << "  Size n: "; cin >> n;
            Matrix<Complex> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            processEigen(A);
        } else {
            int n; cerr << "  Size n: "; cin >> n;
            Matrix<double> A(n, n); cerr << "  Enter A: "; if (!readMatrixFromLine(A)) return;
            processEigen(A);
        }
    }
}

// ===================================================================
//  SECTION 7: Least Squares Polynomial Fitting
// ===================================================================

void leastSquaresFit() {
    separator();
    cerr << "  7.1  Least Squares Polynomial Fitting\n";
    

    string filename;
    if (!g_cli_file.empty()) {
        filename = g_cli_file; g_cli_file.clear();
    } else {
        cerr << "  Enter data file path (x y pairs): ";
        cin >> filename;
    }

    try {
        ifstream file(filename);
        if (!file.is_open()) throw runtime_error("Failed to open file!");

        // Count points
        int n = 0; double tx, ty;
        while (file >> tx >> ty) n++;
        if (n == 0) throw runtime_error("No data points found!");
        file.clear(); file.seekg(0);

        Vector<double> x_vals(n), y_vals(n);
        for (int i = 0; i < n; ++i) { file >> tx >> ty; x_vals(i, 0) = tx; y_vals(i, 0) = ty; }
        file.close();
        cout << "Read " << n << " points.\n";

        int degree;
        cerr << "  Polynomial degree: "; cin >> degree;
        if (degree < 0) throw runtime_error("Degree cannot be negative!");
        if (degree >= n) cout << "Warning: degree >= n, overfitting!\n";

        int m = degree + 1;
        Matrix<double> A(n, m);
        for (int i = 0; i < n; ++i) {
            double x = x_vals(i, 0);
            for (int j = 0; j < m; ++j) A(i, j) = pow(x, j);
        }

        Vector<double> coeffs = A.solveLinear(y_vals);
        if (coeffs.rows() == 0) throw runtime_error("Solve failed!");

        // RMSE
        double sum_sq = 0;
        for (int i = 0; i < n; ++i) {
            double x = x_vals(i, 0), y_pred = 0;
            for (int j = 0; j < m; ++j) y_pred += coeffs(j, 0) * pow(x, j);
            sum_sq += pow(y_vals(i, 0) - y_pred, 2);
        }
        double rmse = sqrt(sum_sq / n);

        // Print equation
        cout << "\nFitted polynomial: y = ";
        bool first = true;
        for (int i = 0; i < m; ++i) {
            double c = coeffs(i, 0);
            if (fabs(c) < 1e-10) continue;
            if (!first) cout << (c > 0 ? " + " : " - ");
            else if (c < 0) cout << "-";
            first = false;
            if (i == 0) cout << fabs(c);
            else if (i == 1) cout << fabs(c) << "x";
            else cout << fabs(c) << "x^" << i;
        }
        cout << "\nRMSE = " << rmse << "\n";

    } catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

// ===================================================================
//  SECTION 8: Matrix Multiplication Performance Benchmark
// ===================================================================

void matrixMulBenchmark() {
    // Standalone implementation for fair benchmarking (same as original)
    const int BLOCK_SIZE = 64;
    const int MAX_THREADS = thread::hardware_concurrency();

    struct BenchMatrix {
        int n;
        vector<vector<double>> data;
        BenchMatrix(int size) : n(size), data(size, vector<double>(size, 0.0)) {}
        double& operator()(int i, int j) { return data[i][j]; }
        const double& operator()(int i, int j) const { return data[i][j]; }
        int size() const { return n; }
        void randomInit() {
            for (int i = 0; i < n; ++i)
                for (int j = 0; j < n; ++j)
                    data[i][j] = rand() % 100 / 10.0;
        }
    };

    auto naiveMul = [](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                for (int k = 0; k < n; ++k)
                    C(i, j) += A(i, k) * B(k, j);
        return C;
    };

    auto blockMul = [&](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        for (int i = 0; i < n; i += BLOCK_SIZE)
            for (int j = 0; j < n; j += BLOCK_SIZE)
                for (int k = 0; k < n; k += BLOCK_SIZE)
                    for (int ii = i; ii < min(i + BLOCK_SIZE, n); ++ii)
                        for (int jj = j; jj < min(j + BLOCK_SIZE, n); ++jj)
                            for (int kk = k; kk < min(k + BLOCK_SIZE, n); ++kk)
                                C(ii, jj) += A(ii, kk) * B(kk, jj);
        return C;
    };

    auto threadedMul = [&](const BenchMatrix& A, const BenchMatrix& B) {
        int n = A.size(); BenchMatrix C(n);
        vector<thread> threads;
        auto worker = [&](int sr, int er) {
            for (int i = sr; i < er; ++i)
                for (int j = 0; j < n; j += BLOCK_SIZE)
                    for (int k = 0; k < n; k += BLOCK_SIZE)
                        for (int jj = j; jj < min(j + BLOCK_SIZE, n); ++jj)
                            for (int kk = k; kk < min(k + BLOCK_SIZE, n); ++kk)
                                C(i, jj) += A(i, kk) * B(kk, jj);
        };
        int rpt = n / MAX_THREADS;
        for (int t = 0; t < MAX_THREADS; ++t) {
            int sr = t * rpt, er = (t == MAX_THREADS - 1) ? n : sr + rpt;
            threads.emplace_back(worker, sr, er);
        }
        for (auto& th : threads) th.join();
        return C;
    };

    separator();
    cerr << "  8.1  Matrix Multiplication Performance Benchmark\n";
    

    srand((unsigned)time(0));
    vector<int> sizes = {256, 512, 1024};

    for (int sz : sizes) {
        cout << "\n  Matrix Size: " << sz << "x" << sz << "\n";
        cout << "  " << string(45, '-') << "\n";

        BenchMatrix A(sz), B(sz);
        A.randomInit(); B.randomInit();

        auto t0 = chrono::high_resolution_clock::now();
        naiveMul(A, B);
        auto t1 = chrono::high_resolution_clock::now();
        double t_naive = chrono::duration<double>(t1 - t0).count();

        t0 = chrono::high_resolution_clock::now();
        blockMul(A, B);
        t1 = chrono::high_resolution_clock::now();
        double t_block = chrono::duration<double>(t1 - t0).count();

        t0 = chrono::high_resolution_clock::now();
        threadedMul(A, B);
        t1 = chrono::high_resolution_clock::now();
        double t_thread = chrono::duration<double>(t1 - t0).count();

        cout << "    Naive:          " << setw(8) << t_naive << " s\n";
        cout << "    Block:          " << setw(8) << t_block << " s  (" << t_naive / t_block << "x)\n";
        cout << "    Threaded+Block: " << setw(8) << t_thread << " s  (" << t_naive / t_thread << "x)\n";
    }
    cout << "\n  Benchmark complete.\n";
}

// ===================================================================
//  MAIN MENU
// ===================================================================

// CLI dispatch helper: maps (section, sub) to function call
static bool runDirect(int sec, int sub) {
    switch (sec) {
        case 1: g_tee->reset(); g_tee->on(); if (sub==1) matrixAdd(); else if (sub==2) matrixSub();
                else if (sub==3) matrixMul(); else if (sub==4) matrixScalarOp();
                else if (sub==5) matrixVectorMul(); else { g_tee->off(); return false; } g_tee->off(); break;
        case 2: g_tee->reset(); g_tee->on(); if (sub==1) vectorAdd(); else if (sub==2) vectorSub();
                else if (sub==3) vectorDot(); else if (sub==4) vectorCross();
                else if (sub==5) vectorScalarMul(); else if (sub==6) vectorNorms();
                else { g_tee->off(); return false; } g_tee->off(); break;
        case 3: g_tee->reset(); g_tee->on(); if (sub==1) matrixDeterminant(); else if (sub==2) matrixAdjugateInverse();
                else if (sub==3) matrixTranspose(); else if (sub==4) matrixRankRREF();
                else { g_tee->off(); return false; } g_tee->off(); break;
        case 4: g_tee->reset(); g_tee->on(); if (sub==1) matrixFrobeniusNorm(); else if (sub==2) matrixRowNorm();
                else if (sub==3) matrixColNorm(); else if (sub==4) matrixSpectralNorm();
                else if (sub==5) matrixCondition(); else { g_tee->off(); return false; } g_tee->off(); break;
        case 5: g_tee->reset(); g_tee->on(); linearSolve(); g_tee->off(); break;
        case 6: g_tee->reset(); g_tee->on(); eigenSolve(); g_tee->off(); break;
        case 7: g_tee->reset(); g_tee->on(); leastSquaresFit(); g_tee->off(); break;
        case 8: g_tee->reset(); g_tee->on(); matrixMulBenchmark(); g_tee->off(); break;
        default: return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    cout << fixed << setprecision(4);
    TeeBuffer __tee("output");   // each reset() creates a new timestamped file
    g_tee = &__tee;
    g_tee->off();

    // ─── CLI direct mode: matrix_cli.exe <section.sub> [file] ───
    if (argc >= 2) {
        double d = atof(argv[1]);
        int sec = (int)floor(d);
        int sub = (d == floor(d)) ? 0 : (int)round((d - floor(d)) * 10);

        if (argc >= 3) {
            g_cli_file = argv[2];                 // provide file to promptFilename()
        } else if (sec >= 1 && sec <= 7) {
            cerr << "Usage: matrix_cli.exe " << argv[1] << " <input_file>\n";
            return 1;
        }

        if (runDirect(sec, sub > 0 ? sub : 0)) {
            if (sec == 8 && argc >= 3) g_cli_file.clear();
            cerr << "\n  (Output saved to output/)\n";
            return 0;
        }
        cerr << "Unknown section: " << argv[1] << "\n";
        return 1;
    }

    // ─── Interactive mode (no CLI args) ───
    while (true) {
        cout << "\n";
        cout << "  " << string(44, '=') << "\n";
        cout << "   MATRIX LINEAR ALGEBRA TOOLKIT (CLI)\n";
        cout << "  " << string(44, '=') << "\n\n";
        cout << "  [1]  Matrix Arithmetic\n";
        cout << "        1.1 Add     1.2 Sub     1.3 Mul\n";
        cout << "        1.4 Scalar  1.5 Mat*Vec\n\n";
        cout << "  [2]  Vector Arithmetic\n";
        cout << "        2.1 Add     2.2 Sub     2.3 Dot\n";
        cout << "        2.4 Cross   2.5 Scalar  2.6 Norms\n\n";
        cout << "  [3]  Core Matrix Operations\n";
        cout << "        3.1 Det     3.2 Adj+Inv\n";
        cout << "        3.3 Trans   3.4 Rank+RREF\n\n";
        cout << "  [4]  Matrix Norms & Condition\n";
        cout << "        4.1 Frobenius    4.2 Row(inf)\n";
        cout << "        4.3 Col(1)       4.4 Spectral\n";
        cout << "        4.5 Condition Number\n\n";
        cout << "  [5]  Linear System Solver  (Ax = b)\n\n";
        cout << "  [6]  Eigenvalues & Eigenvectors\n\n";
        cout << "  [7]  Least Squares Polynomial Fitting\n\n";
        cout << "  [8]  Matrix Multiplication Benchmark\n\n";
        cout << "  [0]  Exit\n";
        cout << "\n  Select >> ";

        // Safely read section number — flush ALL leftover chars from buffer
        int cmd; cin >> cmd;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (cmd == 0) break;

        // Helper: read sub-option, supports both "2" and "1.2" style input
        auto readSub = []() -> int {
            double d; cin >> d;
            cin.ignore(10000, '\n');
            if (d == floor(d)) return (int)d;           // user typed "2"
            else return (int)round((d - floor(d)) * 10); // user typed "1.2" -> sub=2
        };

        switch (cmd) {
            case 1: {
                g_tee->reset(); g_tee->on();
                cerr << "\n  Sub-menu: 1=Add  2=Sub  3=Mul  4=Scalar  5=MatxVec >> ";
                int sub = readSub();
                if (sub == 1) matrixAdd();
                else if (sub == 2) matrixSub();
                else if (sub == 3) matrixMul();
                else if (sub == 4) matrixScalarOp();
                else if (sub == 5) matrixVectorMul();
                g_tee->off();
                break;
            }
            case 2: {
                g_tee->reset(); g_tee->on();
                cerr << "\n  Sub-menu: 1=Add  2=Sub  3=Dot  4=Cross  5=Scalar  6=Norms >> ";
                int sub = readSub();
                if (sub == 1) vectorAdd();
                else if (sub == 2) vectorSub();
                else if (sub == 3) vectorDot();
                else if (sub == 4) vectorCross();
                else if (sub == 5) vectorScalarMul();
                else if (sub == 6) vectorNorms();
                g_tee->off();
                break;
            }
            case 3: {
                g_tee->reset(); g_tee->on();
                cerr << "\n  Sub-menu: 1=Det  2=Adj+Inv  3=Trans  4=Rank+RREF >> ";
                int sub = readSub();
                if (sub == 1) matrixDeterminant();
                else if (sub == 2) matrixAdjugateInverse();
                else if (sub == 3) matrixTranspose();
                else if (sub == 4) matrixRankRREF();
                g_tee->off();
                break;
            }
            case 4: {
                g_tee->reset(); g_tee->on();
                cerr << "\n  Sub-menu: 1=Frob  2=Row  3=Col  4=Spectral  5=Cond >> ";
                int sub = readSub();
                if (sub == 1) matrixFrobeniusNorm();
                else if (sub == 2) matrixRowNorm();
                else if (sub == 3) matrixColNorm();
                else if (sub == 4) matrixSpectralNorm();
                else if (sub == 5) matrixCondition();
                g_tee->off();
                break;
            }
            case 5: g_tee->reset(); g_tee->on(); linearSolve(); g_tee->off(); break;
            case 6: g_tee->reset(); g_tee->on(); eigenSolve(); g_tee->off(); break;
            case 7: g_tee->reset(); g_tee->on(); leastSquaresFit(); g_tee->off(); break;
            case 8: g_tee->reset(); g_tee->on(); g_tee->reset(); g_tee->on(); matrixMulBenchmark(); g_tee->off(); g_tee->off(); break;
        }
        if (cmd >= 1 && cmd <= 8)
            cerr << "\n  (Output saved to output/)\n";
    }

    cout << "\n  Goodbye!\n";
    return 0;
}
