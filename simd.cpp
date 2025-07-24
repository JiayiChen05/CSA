#include <iostream>
#include <chrono>
typedef int v3simd __attribute__ ((vector_size(16)));
int main(){
    v3simd a = {1, 2, 3, 4};
    v3simd b = {1, 2, 3, 4};
    v3simd c = a + b;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 4; ++i){
        printf("C is: %d\n", c[i]);
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Addition took " << duration.count() << " nanoseconds.\n";
    return 0;
}