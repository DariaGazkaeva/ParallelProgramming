#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    const int PROCESS_ZERO = 0;
    const int PROCESS_ONE = 1;
    unsigned char c = 1;
    double t1, t2, t3, dt1;

    for (int n = 100; n <= 1000000; n *= 10) {
        if (rank == PROCESS_ZERO) {
            printf("N = %d \n", n);
        }

        unsigned char message[n];
        for (int i = 0; i < n; i++) {
            message[i] = c;
        }

        if (rank == PROCESS_ZERO) {
            t1 = MPI_Wtime();
            MPI_Send(&message[0], n, MPI_UNSIGNED_CHAR, PROCESS_ONE, 0, MPI_COMM_WORLD);
            unsigned char message_from_one[n];
            MPI_Recv(&message_from_one[0], n, MPI_UNSIGNED_CHAR, PROCESS_ONE, 1, MPI_COMM_WORLD, &status);
            t2 = MPI_Wtime();
            dt1 = t2 - t1;
            printf("Two Way. Time = %f. \n", dt1);
        } else if (rank == PROCESS_ONE) {
            unsigned char message_from_zero[n];
            MPI_Recv(&message_from_zero[0], n, MPI_UNSIGNED_CHAR, PROCESS_ZERO, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&message_from_zero[0], n, MPI_UNSIGNED_CHAR, PROCESS_ZERO, 1, MPI_COMM_WORLD);
        }

        unsigned char message_received[n];

        MPI_Request request1;
        MPI_Request request2;

        t1 = MPI_Wtime();
        MPI_Isend(&message[0], n, MPI_UNSIGNED_CHAR, 1 - rank, 0, MPI_COMM_WORLD, &request1);
        MPI_Irecv(&message_received[0], n, MPI_UNSIGNED_CHAR, 1 - rank, 0, MPI_COMM_WORLD, &request2);
        MPI_Wait(&request1, &status);
        MPI_Wait(&request2, &status);
        t2 = MPI_Wtime();

        MPI_Send(&t2, 1, MPI_DOUBLE, 1 - rank, 1, MPI_COMM_WORLD);
        MPI_Recv(&t3, 1, MPI_DOUBLE, 1 - rank, 1, MPI_COMM_WORLD, &status);
        dt1 = std::abs(t3 - t1);
        printf("One Way. Time = %f. Process %d. \n", dt1, rank);

    }
    MPI_Finalize();
}
