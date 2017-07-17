#include <stdio.h>
#include "routine.h"
#include "MKLSolving.h"

#include <time.h>

int main(int argc, char ** argv)
{
    int i;
    int n;
    int * column;
    int * row;
	int *iPerm;
    FLOAT_TYPE *val;
    int m;
    FLOAT_TYPE *vectorB;
    FLOAT_TYPE *vectorX;
    FLOAT_TYPE *calculationError;
    int error;
    int typeFile;
    int s,f;
    if(argc < 4)
    {
        printf("SolvSystem.exe <typeFile> <matrix A> <vector b> [<out vector x>]\n\n");
        printf("  typeFile:\n");
        printf("  0 - mtx (text file)\n");
        printf("  1 - binary format \n");
        return 1;
    }
    
    s = clock();

    typeFile = atoi(argv[1]);
    parserSystem(typeFile, argv[2], argv[3], &n, &column, &row, &val, &m, &vectorB, &vectorX);

    f = clock();
    printf("Time parser: %f \n", (double)(f - s) / CLOCKS_PER_SEC);

    printf("n :%d \n", n);

	ReadPermutationFromFile(argv[4],&iPerm,n);

    s = clock();
    MKL_SparseSolving(n, m, column, row, val, vectorB, vectorX, iPerm);
    f = clock();
    printf("Time total: %f \n", (double)(f - s) / CLOCKS_PER_SEC);

	printSolution(argv[5], n, m, vectorX);
    testSolution(n, column, row, val, m, vectorB, vectorX, &calculationError);

    printf("Calculation Error: \n");

    for(i = 0; i < m; i++)
    {
        printf("b [%d]:%2.10f \n", i, calculationError[i]);
    }

    //if(argc >= 5)
    //{
    //    printSolution(argv[4], n, m, vectorX);
    //}

    FreeMatrix(&column, &row, &val);
    freeVector(&vectorB);
    freeVector(&vectorX);
    freeVector(&calculationError);

    return 0;
}