#include <iostream>

int main()
{
    int min, max;
    srand(9);

    int d[6][8];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            d[i][j] = rand() % 100;
//            printf("%d ", d[i][j]);
        }
    }
//    printf("\n");

    min = d[0][0];
    max = d[0][0];

#pragma omp parallel for collapse(2) num_threads(2)
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            if (d[i][j] > max) {
#pragma omp critical
                max = d[i][j];
            }

            if (d[i][j] < min) {
#pragma omp critical
                min = d[i][j];
            }
        }
    }

    printf("min = %d, max = %d", min, max);
}
