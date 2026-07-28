#ifndef UTILS_H
#define UTILS_H

#include "matrix.h"
#include "vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <complex>
#include <chrono>
#include <ctime>

// ═══════════════════════════════════════════════════════════════════
//  TeeBuffer — mirrors cout to file; each reset() creates a new file
// ═══════════════════════════════════════════════════════════════════
class TeeBuffer : public std::streambuf {
    std::streambuf* _old;
    std::ofstream   _file;
    std::string     _dir;
    int _counter = 0;
    bool _on = true;

    std::string makeName() {
        auto now = std::chrono::system_clock::now();
        auto t   = std::chrono::system_clock::to_time_t(now);
        auto tm  = *std::localtime(&t);
        std::ostringstream ss;
        ss << _dir << "/result_"
           << std::setfill('0')
           << std::setw(2) << (tm.tm_year + 1900) % 100
           << std::setw(2) << (tm.tm_mon + 1)
           << std::setw(2) << tm.tm_mday << '_'
           << std::setw(2) << tm.tm_hour
           << std::setw(2) << tm.tm_min
           << std::setw(2) << tm.tm_sec << '_'
           << ++_counter << ".txt";
        return ss.str();
    }

public:
    TeeBuffer(const char* dir) : _dir(dir) {
        _old = std::cout.rdbuf(this);
    }
    ~TeeBuffer() { std::cout.rdbuf(_old); if (_file.is_open()) _file.close(); }
    void reset() { _file.close(); _file.clear(); _file.open(makeName()); _on = true; }
    void on()    { _on = true; }
    void off()   { _on = false; _file.flush(); }
protected:
    int overflow(int c) override {
        if (c != EOF) { _old->sputc((char)c); if (_on && _file) _file.put((char)c); }
        return c;
    }
    std::streamsize xsputn(const char* s, std::streamsize n) override {
        std::streamsize r = _old->sputn(s, n);
        if (_on && _file) _file.write(s, n);
        return r;
    }
};

inline TeeBuffer* g_tee = nullptr;

// ═══════════════════════════════════════════════════════════════════
//  File detection & matrix loading
// ═══════════════════════════════════════════════════════════════════

inline bool isComplexFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string token;
    while (file >> token)
        if (token.find('(') != std::string::npos) return true;
    return false;
}

inline bool loadRealMatrix(std::ifstream& file, Matrix<double>& mat) {
    int rows, cols;
    if (!(file >> rows >> cols)) return false;
    mat = Matrix<double>(rows, cols);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (!(file >> mat(i, j))) return false;
    return true;
}

inline bool loadComplexMatrix(std::ifstream& file, Matrix<std::complex<double>>& mat) {
    int rows, cols;
    if (!(file >> rows >> cols)) return false;
    mat = Matrix<std::complex<double>>(rows, cols);
    char ch; double re, im;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            file >> ch >> re >> ch >> im >> ch;
            mat(i, j) = std::complex<double>(re, im);
        }
    return true;
}

// ═══════════════════════════════════════════════════════════════════
//  Stream-based vector reading (shared ifstream — no re-open)
// ═══════════════════════════════════════════════════════════════════

inline Vector<std::complex<double>> readComplexVec(std::ifstream& f) {
    int d; f >> d;
    Vector<std::complex<double>> v(d);
    for (int i = 0; i < d; ++i) {
        char ch; double re, im;
        f >> ch >> re >> ch >> im >> ch;
        v(i, 0) = std::complex<double>(re, im);
    }
    return v;
}

inline Vector<double> readRealVec(std::ifstream& f) {
    int d; f >> d;
    Vector<double> v(d);
    for (int i = 0; i < d; ++i) f >> v(i, 0);
    return v;
}

// ═══════════════════════════════════════════════════════════════════
//  Interactive line-based vector input (count validation)
// ═══════════════════════════════════════════════════════════════════

inline bool checkExtraInput() {
    std::string rest; std::getline(std::cin, rest);
    for (char c : rest) if (!isspace((unsigned char)c))
        { std::cout << "Error: Too many values!\n"; return false; }
    return true;
}

// Read exactly N real values from a line
template<typename T>
bool readVectorFromLine(int dim, Vector<T>& v) {
    std::string line;
    while (line.empty()) std::getline(std::cin, line);
    std::istringstream ss(line);
    for (int i = 0; i < dim; ++i)
        if (!(ss >> v(i, 0))) {
            std::cout << "Error: Need " << dim << " values!\n";
            return false;
        }
    std::string rest; ss >> rest;
    if (!rest.empty()) { std::cout << "Error: Too many values!\n"; return false; }
    return true;
}

// Read exactly N complex values from a line (format: (real,imag))
template<typename T>
bool readComplexVectorFromLine(int dim, Vector<std::complex<T>>& v) {
    std::string line;
    while (line.empty()) std::getline(std::cin, line);
    std::istringstream ss(line);
    for (int i = 0; i < dim; ++i) {
        char ch; T re, im;
        if (!(ss >> ch >> re >> ch >> im >> ch)) {
            std::cout << "Error: Need " << dim << " complex values!\n";
            return false;
        }
        v(i, 0) = std::complex<T>(re, im);
    }
    std::string rest; ss >> rest;
    if (!rest.empty()) { std::cout << "Error: Too many values!\n"; return false; }
    return true;
}

// ═══════════════════════════════════════════════════════════════════
//  Interactive line-based matrix input (count validation)
// ═══════════════════════════════════════════════════════════════════

// Read exactly N real matrix elements from a line
template<typename MatrixType>
bool readMatrixFromLine(MatrixType& m) {
    int n = m.rows() * m.cols();
    std::string line;
    while (line.empty()) std::getline(std::cin, line);
    std::istringstream ss(line);
    for (int i = 0; i < m.rows(); ++i)
        for (int j = 0; j < m.cols(); ++j) {
            if (!(ss >> m(i, j))) {
                std::cout << "Error: Need " << n << " values!\n";
                return false;
            }
        }
    std::string rest; ss >> rest;
    if (!rest.empty()) { std::cout << "Error: Too many values!\n"; return false; }
    return true;
}

// Read exactly N complex matrix elements from a line (format: (real,imag))
template<typename T>
bool readComplexMatrixFromLine(Matrix<std::complex<T>>& m) {
    int n = m.rows() * m.cols();
    std::string line;
    while (line.empty()) std::getline(std::cin, line);
    std::istringstream ss(line);
    for (int i = 0; i < m.rows(); ++i)
        for (int j = 0; j < m.cols(); ++j) {
            char ch; T re, im;
            if (!(ss >> ch >> re >> ch >> im >> ch)) {
                std::cout << "Error: Need " << n << " complex values!\n";
                return false;
            }
            m(i, j) = std::complex<T>(re, im);
        }
    std::string rest; ss >> rest;
    if (!rest.empty()) { std::cout << "Error: Too many values!\n"; return false; }
    return true;
}

#endif
