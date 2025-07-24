#include <iostream>
#include <chrono>

int main(){
    int a[4], b[4], c[4];

    // 初始化 a 和 b
    for (int i = 0; i < 4; ++i) {
        a[i] = i;
        b[i] = i;
    }

    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();

    // sisd:串型加法
    for (int i = 0; i < 4; ++i){
        c[i] = a[i] + b[i];
    }

    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();

    // 输出结果
    for (int i = 0; i < 4; ++i){
        printf("C is: %d\n", c[i]);
    }

    // 计算耗时
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Addition took " << duration.count() << " nanoseconds.\n";

    return 0;
}
