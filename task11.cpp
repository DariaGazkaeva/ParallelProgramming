#include <iostream>

int main()
{
    srand(11);

    int a[30];
    for (int i = 0; i < 30; i++) {
        a[i] = rand() % 100;
        printf("%d ", a[i]);
    }
    printf("\n");

    int max = -1;

#pragma omp parallel for num_threads(2)
    for (int i = 0; i < 30; i++) {
        if (a[i] % 7 == 0 && a[i] > max) {
#pragma omp critical
            max = a[i];
        }
    }

    if (max != -1) {
        printf("Max = %d", max);
    } else {
        printf("No");
    }
}
