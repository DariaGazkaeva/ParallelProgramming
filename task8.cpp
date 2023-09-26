#include <iostream>
#include <chrono>

int main()
{
    int const rows = 10000, columns = 1000;

    int static matrix[rows][columns];
    int static vector[columns];
    int static result[rows];

    float time = 0.0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = i + j;
        }
    }

    for (int i = 0; i < columns; i++) {
        vector[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < columns; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
    printf("Time one: %f \n", time);


    start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < columns; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    end = std::chrono::high_resolution_clock::now();

    time = std::chrono::duration_cast<std::chrono::duration<float>>(end - start).count();
    printf("Time two: %f", time);
}
