#include <iostream>
#include <chrono>
#include <omp.h>

int main() {
    int a[4], b[4], c[4];

    // 初始化数据
    for (int i = 0; i < 4; ++i) {
        a[i] = i;
        b[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // SPMD: 多线程执行同一段代码，但各处理不同数据
    #pragma omp parallel for
    for (int i = 0; i < 4; ++i) {
        c[i] = a[i] + b[i];
    }

    auto end = std::chrono::high_resolution_clock::now();

    // 输出结果
    for (int i = 0; i < 4; ++i) {
        printf("C[%d] = %d\n", i, c[i]);
    }

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "SPMD addition took " << duration.count() << " nanoseconds.\n";

    return 0;
}
