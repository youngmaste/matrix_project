#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace chrono;

// ===================== 配置参数 =====================
const int BLOCK_SIZE = 64;    // 分块大小（适配CPU L1缓存，最优值64/128）
const int MAX_THREADS = thread::hardware_concurrency(); // 获取CPU核心数

// ===================== 矩阵类 =====================
class Matrix {
private:
    int n;              // 方阵大小 n*n
    vector<vector<double>> data;

public:
    // 构造函数
    Matrix(int size) : n(size), data(size, vector<double>(size, 0.0)) {}

    // 赋值运算符
    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            n = other.n;
            data = other.data;
        }
        return *this;
    }

    // 访问元素
    double& operator()(int i, int j) { return data[i][j]; }
    const double& operator()(int i, int j) const { return data[i][j]; }

    int size() const { return n; }

    // 随机初始化矩阵
    void randomInit() {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                data[i][j] = rand() % 100 / 10.0;
    }
};

// ===================== 1. 普通三重循环乘法（基准） =====================
Matrix naiveMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C(i, j) += A(i, k) * B(k, j);
    return C;
}

// ===================== 2. 分块矩阵乘法（Cache优化） =====================
Matrix blockMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n);

    // 分块循环
    for (int i = 0; i < n; i += BLOCK_SIZE)
        for (int j = 0; j < n; j += BLOCK_SIZE)
            for (int k = 0; k < n; k += BLOCK_SIZE)
                // 块内普通乘法
                for (int ii = i; ii < min(i + BLOCK_SIZE, n); ++ii)
                    for (int jj = j; jj < min(j + BLOCK_SIZE, n); ++jj)
                        for (int kk = k; kk < min(k + BLOCK_SIZE, n); ++kk)
                            C(ii, jj) += A(ii, kk) * B(kk, jj);
    return C;
}

// ===================== 3. 多线程分块乘法（并行优化） =====================
// 线程执行函数：计算指定行区间的结果
void multiplySection(const Matrix& A, const Matrix& B, Matrix& C, int startRow, int endRow) {
    int n = A.size();
    for (int i = startRow; i < endRow; ++i)
        for (int j = 0; j < n; j += BLOCK_SIZE)
            for (int k = 0; k < n; k += BLOCK_SIZE)
                for (int jj = j; jj < min(j + BLOCK_SIZE, n); ++jj)
                    for (int kk = k; kk < min(k + BLOCK_SIZE, n); ++kk)
                        C(i, jj) += A(i, kk) * B(kk, jj);
}

Matrix threadedBlockMultiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n);
    vector<thread> threads;

    // 均分任务给每个线程
    int rowsPerThread = n / MAX_THREADS;
    for (int t = 0; t < MAX_THREADS; ++t) {
        int start = t * rowsPerThread;
        int end = (t == MAX_THREADS - 1) ? n : start + rowsPerThread;
        threads.emplace_back(multiplySection, cref(A), cref(B), ref(C), start, end);
    }

    // 等待所有线程完成
    for (auto& th : threads)
        th.join();

    return C;
}

// ===================== 计时函数 =====================
template<typename Func>
double measureTime(Func func, const Matrix& A, const Matrix& B) {
    auto start = high_resolution_clock::now();
    func(A, B);
    auto end = high_resolution_clock::now();
    duration<double> elapsed = end - start;
    return elapsed.count(); // 返回秒数
}

// ===================== 主函数：性能测试 =====================
int main() {
    srand(time(0));
    cout << fixed << setprecision(4);
    cout << "===== Matrix Multiplication Performance Test =====" << endl;
    cout << "CPU Threads: " << MAX_THREADS << endl;
    cout << "Block Size: " << BLOCK_SIZE << endl << endl;

    // 测试矩阵规模（可修改：512/1024/2048，规模越大优化效果越明显）
    vector<int> testSizes = {256, 512, 1024};

    for (int size : testSizes) {
        cout << "========================================" << endl;
        cout << "Matrix Size: " << size << "x" << size << endl;

        Matrix A(size), B(size);
        A.randomInit();
        B.randomInit();

        // 测试三种方法
        double t_naive = measureTime(naiveMultiply, A, B);
        double t_block = measureTime(blockMultiply, A, B);
        double t_thread = measureTime(threadedBlockMultiply, A, B);

        // 输出结果
        cout << "Naive Multiply:   " << t_naive << " s" << endl;
        cout << "Block Multiply:   " << t_block << " s" << endl;
        cout << "Threaded Block:   " << t_thread << " s" << endl;

        // 加速比
        cout << "Block Speed Up:   " << t_naive / t_block << "x" << endl;
        cout << "Thread Speed Up:  " << t_naive / t_thread << "x" << endl;
        cout << endl;
    }

    cout << "Test completed!" << endl;
    return 0;
}