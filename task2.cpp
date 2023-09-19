#include <iostream>
#include <omp.h>

int main(int argc, char **argv) {
    int countThreads = 3;

#pragma omp parallel if (countThreads > 1) num_threads(countThreads)
    {
        printf("First. ID: %d, Count of Threads: %d \n", omp_get_thread_num(), omp_get_num_threads());
    }

    countThreads = 1;

#pragma omp parallel if (countThreads > 1) num_threads(countThreads)
    {
        printf("Second. ID: %d, Count of Threads: %d \n", omp_get_thread_num(), omp_get_num_threads());
    }

    return 0;
}
