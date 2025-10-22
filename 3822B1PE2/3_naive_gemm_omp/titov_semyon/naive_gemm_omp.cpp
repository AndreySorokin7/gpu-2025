#include "naive_gemm_omp.h"
#include <vector>
#include <cstring>

std::vector<float> NaiveGemmOMP(const std::vector<float>& a,
    const std::vector<float>& b,
    int n) {
    std::vector<float> c(n * n, 0.0f);

    std::vector<float> b_transposed(n * n);
#pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b_transposed[j * n + i] = b[i * n + j];
        }
    }

#pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            float sum = 0.0f;
            int k = 0;
            for (; k <= n - 4; k += 4) {
                sum += a[i * n + k] * b_transposed[j * n + k] +
                    a[i * n + k + 1] * b_transposed[j * n + k + 1] +
                    a[i * n + k + 2] * b_transposed[j * n + k + 2] +
                    a[i * n + k + 3] * b_transposed[j * n + k + 3];
            }
            for (; k < n; k++) {
                sum += a[i * n + k] * b_transposed[j * n + k];
            }
            c[i * n + j] = sum;
        }
    }

    return c;
}