#include <vector>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int select_pivot(int arr[], int start, int stop) {
    const int size = stop - start + 1;

    if (size < 3) {
        return stop;
    }

    const int middle = start + (stop - start) / 2;

    if (arr[start] > arr[middle]) {
        if (arr[middle] > arr[stop]) {
            return middle;
        } else if (arr[start] > arr[stop]) {
            return stop;
        } else {
            return start;
        }
    } else {
        if (arr[start] > arr[stop]) {
            return start;
        } else if (arr[middle] > arr[stop]) {
            return stop;
        } else {
            return middle;
        }
    }
}

int partition(int arr[], int start, int stop, int pivot) {
    const int pivot_value = arr[pivot];
    std::swap(arr[pivot], arr[stop]);
    int curr_pivot_index = start;

    for (int index = start; index < stop; index++) {
        if (arr[index] < pivot_value) {
            if (index != curr_pivot_index) {
                std::swap(arr[index], arr[curr_pivot_index]);
            }
            curr_pivot_index++;
        }
    }
    std::swap(arr[curr_pivot_index], arr[stop]);
    return curr_pivot_index;
}

void quick_sort_serial(int arr[], int start, int stop) {
    if (start >= stop) {
        return;
    }
    int pivot1 = select_pivot(arr, start, stop);
    int pivot2 = partition(arr, start, stop, pivot1);

    quick_sort_serial(arr, start, pivot2 - 1);
    quick_sort_serial(arr, pivot2 + 1, stop);
}

void quick_sort_parallel(int arr[], int n) {
    if (n <= 1) {
        return;
    }
    int start = 0;
    int stop = n - 1;
    int pivot_index = select_pivot(arr, start, stop);
    int new_pivot_index = partition(arr, start, stop, pivot_index);
    start = new_pivot_index + 1;
    stop = new_pivot_index - 1;
#pragma omp task shared(arr)
    {
        if (stop > 0) {
            quick_sort_parallel(arr, stop + 1);
        }
    }
    if (n > start) {
        quick_sort_parallel(arr + start, n - start);
    }
}

void func(int len, int n_threads) {
    int* arr = (int*) malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        arr[i] = rand() % 100;
    }
    double start = omp_get_wtime();
    omp_set_num_threads(n_threads);
#pragma omp parallel
    {
#pragma omp single
        quick_sort_parallel(arr, len);
    }
    double end = omp_get_wtime();
    free(arr);
    printf("N threads = %d, Size of array = %d, Time = %f\n", n_threads, len, end - start);
}

int main() {
    const int N = 5;
    const int maxCountOfThread = 10;
    int arraySizes[N] = {50000, 100000, 300000, 400000, 500000 };

    for (int i = 0; i < N; i++) {
        for (int n_threads = 1; n_threads <= maxCountOfThread; n_threads++) {
            func(arraySizes[i], n_threads);
        }
        printf("\n");
    }
}
