#include <iostream>
#include <climits>
#include <omp.h>

int main() {
    int a[10] = {5, 2, 8, 1, 6, 9, 3, 7, 4, 10};
    int b[10] = {14, 9, 5, 18, 2, 11, 7, 6, 13, 1};

    int minA = INT_MAX;
    int maxB = INT_MIN;

#pragma omp parallel num_threads(2)
    {
        int num = omp_get_thread_num();

        if (num == 0) {
            for (int i = 0; i < 10; ++i) {
                if (a[i] < minA) {
                    minA = a[i];
                }
            }
        } else if (num == 1) {
            for (int i = 0; i < 10; ++i) {
                if (b[i] > maxB) {
                    maxB = b[i];
                }
            }
        }
    }
    printf("Min value in array a: %d\n", minA);
    printf("Max value in array b: %d\n", maxB);

    return 0;
}
