#include <iostream>
#include <math.h>

int main() {

    int a[] = {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};
    int x = std::distance(std::begin(a), std::end(a));
    int num = 0;

#pragma omp parallel for reduction(+:num)
    for (int i = 0; i < x; i++) {
        if (a[i] == 1) {
            num += pow(2, x - 1 - i);
        }
    }

    printf("%d", num);
}
