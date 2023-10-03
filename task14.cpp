#include <iostream>

int main() {

    int num = 210;
    int square = 0;

#pragma omp parallel for reduction(+:square)
    for (int i = 1; i < num * 2; i++) {
        if (i % 2 == 1) {
            square += i;
        }
    }

    printf("%d", square);
}
