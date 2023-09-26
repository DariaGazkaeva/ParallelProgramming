#include <iostream>

int main()
{
    int a[30];
    srand(10);

    for (int i = 0; i < 30; i++) {
        a[i] = rand() % 100;
//        printf("%d ", a[i]);
    }
//    printf("\n");

    int count = 0;

#pragma omp parallel for num_threads(2)
    for (int i = 0; i < 30; i++) {
//        printf("%d ", (a[i] % 9));
        if (a[i] % 9 == 0) {
#pragma omp atomic
            count++;
        }
    }

    printf("Count: %d", count);
}
