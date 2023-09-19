#include <iostream>
#include <omp.h>

int main() {
    int d[6][8];

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 8; ++j) {
            d[i][j] = rand() % 100;
            std::cout << d[i][j] << " ";
        }
        std::cout << "\n";
    };

    float average = 0.0;
    int min = INT_MAX;
    int max = INT_MIN;
    int count = 0;

    #pragma omp parallel sections
        {
        #pragma omp section
            {
                int num = omp_get_thread_num();
                int totalElements = 6 * 8;

                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        average += d[i][j];
                    }
                }
                average /= totalElements;
                printf("Thread: %d, Average: %f, \n", num, average);
            }

            #pragma omp section
            {
                int num = omp_get_thread_num();

                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
                        if (d[i][j] < min) {
                            min = d[i][j];
                        }

                        if (d[i][j] > max) {
                            max = d[i][j];
                        }
                    }
                }
                printf("Thread: %d, Min: %d, Max: %d\n", num, min, max);
            }

            #pragma omp section
                {
                    int num = omp_get_thread_num();

                    for (int i = 0; i < 6; ++i) {
                        for (int j = 0; j < 8; ++j) {
                            if (d[i][j] % 3 == 0) {
                                ++count;
                            }
                        }
                    }
                    printf("Thread: %d, Count: %d \n", num, count);
                }
        }

    return 0;
}
