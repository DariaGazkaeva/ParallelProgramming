#include <iostream>

int main()
{
    int a[10] = {5, 2, 8, 1, 6, 9, 3, 7, 4, 10};
    int b[10] = {14, 9, 5, 18, 2, 11, 7, 6, 13, 1};

    float avg_a = 0;
    float avg_b = 0;

#pragma omp parallel for reduction(+: avg_a) reduction(+: avg_b)
    for (int i = 0; i < 10; i++) {
        avg_a += a[i];
        avg_b += b[i];
    }

    avg_a = avg_a / 10;
    avg_b = avg_b / 10;

    printf("avg_a = %f, avg_b  = %f \n", avg_a, avg_b);

    if (avg_a > avg_b) {
        printf("avg_a more than avg_b \n");
    } else if (avg_a < avg_b) {
        printf("avg_b more than avg_a \n");
    } else {
        printf("avg_a equals avg_b \n");
    }
}

