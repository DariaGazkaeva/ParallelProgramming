#include <mpi.h>
#include <random>

int main(int argc, char** argv)
{
    int rank, count;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    const int ZERO_PROCESS = 0;
    const int DIM = 10;
    const int BLOCK = 10 * 5;

    if (rank == ZERO_PROCESS) {
        int A[DIM][DIM];
        int A_transposed[DIM][DIM];

        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                A[i][j] = rand() % 10;
            }
        }

        printf("Matrix A: \n");
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }

        int dest = 1;
        for (int i = 0; i < DIM; i += DIM / 2) {
            for (int j = 0; j < DIM; j += DIM, dest++) {
                MPI_Send(&A[i][j], BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
            }
        }

        int A_part[10][5];
        for (int source = 1; source < 3; source++) {
            MPI_Recv(&A_part[0][0], BLOCK, MPI_INT, source, 0, MPI_COMM_WORLD, &status);

            for (int i = 0; i < DIM; i++) {
                if (source == 1) {
                    for (int j = 0; j < DIM / 2; j++) {
                        A_transposed[i][j] = A_part[i][j];
                    }
                } else {
                    for (int j = DIM / 2, q = 0; j < DIM; j++, q++) {
                        A_transposed[i][j] = A_part[i][q];
                    }
                }
            }
        }

        printf("Transposed matrix: \n");
        for (int i = 0; i < DIM; i++) {
            for (int j = 0; j < DIM; j++) {
                printf("%d ", A_transposed[i][j]);
            }
            printf("\n");
        }
    }
    else {
        MPI_Probe(ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &count);
        int A[5][10];
        int A_transposed[10][5];

        MPI_Recv(&A[0][0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD, &status);

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 10; j++) {
                A_transposed[j][i] = A[i][j];
            }
        }

        MPI_Send(&A_transposed[0][0], count, MPI_INT, ZERO_PROCESS, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

//#include <mpi.h>
//#include <random>
//
//int main(int argc, char** argv)
//{
//    int rank, count;
//    MPI_Init(&argc, &argv);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    MPI_Status status;
//    const int DIM = 10;
//    const int BLOCK = 10;
//
//    if (rank == 0) {
//        int A[DIM][DIM];
//        int A_transposed[DIM][DIM];
//
//        for (int i = 0; i < DIM; i++) {
//            for (int j = 0; j < DIM; j++) {
//                A[i][j] = rand() % 10;
//            }
//        }
//
//        printf("Matrix A: \n");
//        for (int i = 0; i < DIM; i++) {
//            for (int j = 0; j < DIM; j++) {
//                printf("%d ", A[i][j]);
//            }
//            printf("\n");
//        }
//
//        int dest = 1;
//        for (int i = 0; i < DIM; i += BLOCK) {
//            for (int j = 0; j < DIM; j += BLOCK, dest++) {
//                MPI_Send(&A[i][j], BLOCK * BLOCK, MPI_INT, dest, 0, MPI_COMM_WORLD);
//            }
//        }
//
//        int source = 1;
//        for (int i = 0; i < DIM; i += BLOCK) {
//            for (int j = 0; j < DIM; j += BLOCK, source++) {
//                MPI_Recv(&A_transposed[i][j], BLOCK * BLOCK, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
//            }
//        }
//
//        printf("Transposed matrix: \n");
//        for (int i = 0; i < DIM; i++) {
//            for (int j = 0; j < DIM; j++) {
//                printf("%d ", A_transposed[i][j]);
//            }
//            printf("\n");
//        }
//    }
//    else {
//        MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
//        MPI_Get_count(&status, MPI_INT, &count);
//        int A[BLOCK][BLOCK];
//        int A_transposed[BLOCK][BLOCK];
//
//        MPI_Recv(&A[0][0], count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//
//        for (int i = 0; i < BLOCK; i++) {
//            for (int j = 0; j < BLOCK; j++) {
//                A_transposed[j][i] = A[i][j];
//            }
//        }
//
//        MPI_Send(&A_transposed[0][0], count, MPI_INT, 0, 0, MPI_COMM_WORLD);
//    }
//    MPI_Finalize();
//}

