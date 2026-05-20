#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stdexcept>

using namespace std;

// 2D Point alias
using Point = pair<double, double>;

// -------------------------- Utility Functions --------------------------
/// @brief Read 2D points from a text file (each line: x y)
vector<Point> readPointsFromFile(const string& filename) {
    vector<Point> points;
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file! Please check the file path and name.");
    }

    double x, y;
    while (file >> x >> y) {
        points.emplace_back(x, y);
    }
    file.close();

    if (points.empty()) {
        throw runtime_error("No valid data points read from the file!");
    }
    return points;
}

/// @brief Construct Vandermonde matrix
vector<vector<double>> buildVandermondeMatrix(const vector<Point>& points, int degree) {
    int n = points.size();    // Rows of the matrix
    int cols = degree + 1;    // Columns of the matrix
    vector<vector<double>> mat(n, vector<double>(cols, 0.0));

    for (int i = 0; i < n; ++i) {
        double x = points[i].first;
        for (int j = 0; j < cols; ++j) {
            mat[i][j] = pow(x, j);  // A[i][j] = x_i^j
        }
    }
    return mat;
}

/// @brief Matrix transpose
vector<vector<double>> transposeMatrix(const vector<vector<double>>& mat) {
    int rows = mat.size();
    int cols = mat[0].size();
    vector<vector<double>> trans(cols, vector<double>(rows, 0.0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            trans[j][i] = mat[i][j];
        }
    }
    return trans;
}

/// @brief Matrix multiplication: mat1 * mat2
vector<vector<double>> multiplyMatrix(const vector<vector<double>>& mat1, const vector<vector<double>>& mat2) {
    int r1 = mat1.size(), c1 = mat1[0].size();
    int r2 = mat2.size(), c2 = mat2[0].size();
    if (c1 != r2) throw runtime_error("Matrix dimensions do not match, cannot multiply!");

    vector<vector<double>> res(r1, vector<double>(c2, 0.0));
    for (int i = 0; i < r1; ++i) {
        for (int k = 0; k < c1; ++k) {
            if (mat1[i][k] == 0) continue; // Optimization
            for (int j = 0; j < c2; ++j) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}

/// @brief Matrix-vector multiplication: mat * vec
vector<double> multiplyMatrixVector(const vector<vector<double>>& mat, const vector<double>& vec) {
    int rows = mat.size(), cols = mat[0].size();
    if (cols != vec.size()) throw runtime_error("Matrix and vector dimensions do not match!");

    vector<double> res(rows, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            res[i] += mat[i][j] * vec[j];
        }
    }
    return res;
}

/// @brief Gaussian elimination to solve linear system Ax = b
vector<double> gaussElimination(vector<vector<double>> A, vector<double> b) {
    int n = A.size();
    // Construct augmented matrix
    for (int i = 0; i < n; ++i) {
        A[i].push_back(b[i]);
    }

    // Forward elimination
    for (int col = 0; col < n; ++col) {
        // Find pivot (Normal equation matrix is positive definite, no pivot selection needed)
        double pivot = A[col][col];
        if (fabs(pivot) < 1e-10) throw runtime_error("Matrix is singular, cannot solve!");

        // Normalize pivot row
        for (int j = col; j <= n; ++j) {
            A[col][j] /= pivot;
        }

        // Eliminate rows below
        for (int row = col + 1; row < n; ++row) {
            double factor = A[row][col];
            for (int j = col; j <= n; ++j) {
                A[row][j] -= factor * A[col][j];
            }
        }
    }

    // Back substitution
    vector<double> x(n, 0.0);
    for (int i = n - 1; i >= 0; --i) {
        x[i] = A[i][n];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= A[i][j] * x[j];
        }
    }
    return x;
}

/// @brief Calculate RMSE of the fitting
double calculateRMSE(const vector<Point>& points, const vector<double>& coeffs) {
    int n = points.size();
    int m = coeffs.size() - 1;
    double sum_sq = 0.0;

    for (const auto& p : points) {
        double x = p.first, y_true = p.second;
        double y_pred = 0.0;
        // Calculate predicted value y = a0 +a1x +a2x²+...+amx^m
        for (int i = 0; i <= m; ++i) {
            y_pred += coeffs[i] * pow(x, i);
        }
        sum_sq += pow(y_true - y_pred, 2);
    }
    return sqrt(sum_sq / n);
}

/// @brief Print polynomial equation in formatted style
void printPolynomial(const vector<double>& coeffs) {
    int m = coeffs.size() - 1;
    cout << "\nFitting curve equation: y = ";
    bool first = true;

    for (int i = 0; i <= m; ++i) {
        double c = coeffs[i];
        if (fabs(c) < 1e-10) continue; // Skip terms with zero coefficient

        // Sign handling
        if (!first) {
            cout << (c > 0 ? " + " : " - ");
        } else if (c < 0) {
            cout << "-";
        }
        first = false;

        double abs_c = fabs(c);
        // Constant term
        if (i == 0) {
            cout << fixed << setprecision(6) << abs_c;
        }
        // Linear term
        else if (i == 1) {
            cout << fixed << setprecision(6) << abs_c << "x";
        }
        // Higher order terms
        else {
            cout << fixed << setprecision(6) << abs_c << "x^" << i;
        }
    }
    cout << endl;
}

// -------------------------- Main Function --------------------------
int main() {
    // Set output precision
    cout << fixed << setprecision(6);

    try {
        // 1. Read data file
        string filename;
        cout << "Please enter the data file path/name: ";
        cin >> filename;
        vector<Point> points = readPointsFromFile(filename);
        cout << "Successfully read " << points.size() << " data points" << endl;

        // 2. Input fitting degree
        int degree;
        cout << "Please enter the polynomial fitting degree (positive integer): ";
        cin >> degree;
        if (degree < 0) throw runtime_error("Fitting degree cannot be negative!");
        if (degree >= points.size()) {
            cout << "Warning: Fitting degree ≥ number of data points, overfitting will occur!" << endl;
        }

        // 3. Construct Vandermonde matrix A
        vector<vector<double>> A = buildVandermondeMatrix(points, degree);

        // 4. Construct normal equation: A^T A a = A^T y
        vector<vector<double>> AT = transposeMatrix(A);        // Transpose of A
        vector<vector<double>> G = multiplyMatrix(AT, A);      // Coefficient matrix G = A^T A

        // Construct y vector
        vector<double> y(points.size());
        for (int i = 0; i < points.size(); ++i) {
            y[i] = points[i].second;
        }
        vector<double> b = multiplyMatrixVector(AT, y);        // Right-hand vector b = A^T y

        // 5. Solve normal equation to get polynomial coefficients
        vector<double> coeffs = gaussElimination(G, b);

        // 6. Calculate and print results
        printPolynomial(coeffs);
        double rmse = calculateRMSE(points, coeffs);
        cout << "Root Mean Square Error (RMSE) = " << rmse << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}