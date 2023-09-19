#include <iostream>
#include <omp.h>

int main(int argc, char **argv) {
    #pragma omp parallel
    {
        printf("ID: %d, Count of Threads: %d, Hello World \n", omp_get_thread_num(), omp_get_num_threads());
    }
    return 0;
}
