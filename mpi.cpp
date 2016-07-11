#include <stdio.h>
#include <mpi.h>

const double PI = 3.1415926535897932;
const long STEP_NUM = 1000000000;
const double STEP_LENGTH = 1.0 / STEP_NUM;

int main (int argc, char* argv[])
{
    int rank, size;
    double pi, sum = 0.0, result = 0.0;

    MPI_Init (&argc, &argv);

    // get process ID
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);

    // get processes Number
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    // synchronize all processes and get the begin time
    MPI_Barrier(MPI_COMM_WORLD);

    double startTime = MPI_Wtime();

    // each process will caculate a part of the sum
    double x;
    for (int i = rank; i < STEP_NUM; i += size)
    {
        x = (i + 0.5) * STEP_LENGTH;
        result += 1.0 / (1.0 + x * x);
    }

    // sum up all results
    MPI_Reduce(&result, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // synchronize all processes and get the end time
    MPI_Barrier(MPI_COMM_WORLD);
    double endTime = MPI_Wtime();

    // caculate and print PI
    if (rank == 0)
    {
        pi = STEP_LENGTH * pi * 4;
        printf("PI = %.16lf with error %.16lf\nTime elapsed : %lf seconds.\n", pi, PI - pi, (endTime - startTime));
    }

    MPI_Finalize();

    return 0;
}