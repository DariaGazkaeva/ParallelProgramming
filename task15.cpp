#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>


bool is_simple(int n) {
    if (n == 2) {
        return true;
    }
    if (n == 1 || n % 2 == 0) {
        return false;
    }
    int sqrt_n = sqrt(n);
    for (int i = 3; i <= sqrt_n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main() {

    int start;
    int end;
    std::vector<int> res;

    printf("Start:");
    std::cin >> start;
    printf("End:");
    std::cin >> end;

#pragma omp parallel for num_threads(2)
    for (int i = start + 1 - start % 2; i <= end; i += 2)
    {
        if (is_simple(i)) {
#pragma omp critical
            res.push_back(i);
        }
    }

    std::sort(res.begin(), res.end());

    printf("Count: %d \n", res.size());
    for (int i = 0; i < res.size(); i++) {
        printf("%d ", res[i]);
    }
}
