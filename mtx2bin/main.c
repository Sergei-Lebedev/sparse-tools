#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "mmio.h"

void printHelp(){
    printf("Use this program to convert mtx matrices to slasbin format");
    exit(0);
}

int readMTX(const char* fileName, int **I, int **J, double **val, int *M, int *N, int *nz){
    FILE *f;
    int ret_code;
    MM_typecode matcode;
    int i;

    if ((f = fopen(fileName, "r")) == NULL) {
        printf("Can't open file %s\n", fileName);
        return 1;
    }
    else
        printf("Matrix: %s\n", fileName);
    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        return 1;
    }

    if (mm_is_complex(matcode))
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        return 1;
    }

    /* find out size of sparse matrix .... */
    if ((ret_code = mm_read_mtx_crd_size(f, M, N, nz)) !=0)
        return 1;

    (*I) = (int *) malloc((*nz) * sizeof(int));
    (*J) = (int *) malloc((*nz) * sizeof(int));
    (*val) = (double *) malloc((*nz) * sizeof(double));

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */

    for (i=0; i<(*nz); i++)
    {
        fscanf(f, "%d %d %lg\n", &(*I)[i], &(*J)[i], &(*val)[i]);
        (*I)[i]--;  /* adjust from 1-based to 0-based */
        (*J)[i]--;
    }

    if (f !=stdin) fclose(f);

    mm_write_banner(stdout, matcode);
    mm_write_mtx_crd_size(stdout, (*M), (*N), (*nz));
    for (i=0; i<(*nz); i++)
        fprintf(stdout, "%d %d %20.19g\n", (*I)[i]+1, (*J)[i]+1, (*val)[i]);
    return 0;
}

int saveBin(const char* fileName, int *I, int *j, double *val){
    return 0;
}

int main(int argc, char* argv[]){
    int error = 0;
    int *I, *J;
    double *val;
    int M, N, nz;
    if (argc < 2) {
        printHelp();
        exit(0);
    }
    if (readMTX(argv[1], &I, &J, &val, &M, &N, &nz) != 0)
        exit(0);
    free(I);
    free(J);
    free(val);

    return 0;
}