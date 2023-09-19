#include <iostream>
#include <omp.h>

int main(int argc, char **argv) {
    int a = 5;
    int b = 10;

    printf("Before first - a: %d, b: %d \n", a, b);

    #pragma omp parallel num_threads(2) private(a) firstprivate(b)
    {
        int num = omp_get_thread_num();
        a += num;
        b += num;

        printf("First - a: %d, b: %d \n", a, b);
    }

    printf("After first - a: %d, b: %d \n", a, b);

    printf("Before second - a: %d, b: %d \n", a, b);

    #pragma omp parallel num_threads(4) shared(a) private(b)
    {
        int num = omp_get_thread_num();
        a -= num;
        b -= num;

        printf("Second - a: %d, b: %d\n", a, b);
    }

    printf("After second - a: %d, b: %d \n", a, b);

    return 0;
}

