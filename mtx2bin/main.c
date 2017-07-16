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

int COOtoCRS(int n, int nz, int *I, int *J, double *valCOO, int **row, int **col, double **valCRS){
    int i;
    int *places;

    (*row) = (int*)malloc((n+1)*sizeof(int));
    (*col) = (int*)malloc((nz)*sizeof(int));
    (*valCRS) = (double*)malloc((nz)*sizeof(double));
    places = (int*)malloc(n*sizeof(int));

    for(i = 0; i < n+1; i++){
        (*row)[i] = 0;
    }
    for(i = 0; i < nz; i++){
        (*row)[I[i]+1] += 1;
    }
    for(i = 0; i < n; i++){
        (*row)[i+1] += (*row)[i];
        places[i] = (*row)[i];
    }
    for(i = 0; i < nz; i++){
        (*col)[places[I[i]]] = J[i];
        (*valCRS)[places[I[i]]] = valCOO[i];
        places[I[i]]++;
    }
    //sort each column
    for(i = 0; i < n; i++){
        int start, finish, j, k;
        start = (*row)[i];
        finish = (*row)[i+1];
        //need to write bubble sort
        for(j = start; j < finish; j++)
            for(k = start; k < finish -j - 1; k++)
                if ((*col)[k]>(*col)[k+1]){
                    int tempCol = (*col)[k];
                    (*col)[k] = (*col)[k+1];
                    (*col)[k+1] = tempCol;

                    double tempVal = (*valCRS)[k];
                    (*valCRS)[k] = (*valCRS)[k+1];
                    (*valCRS)[k+1] = tempVal;
                }

    }
    free(places);
    return 0;
}
int saveBin(const char* fileName, int *I, int *j, double *val){
    return 0;
}

int main(int argc, char* argv[]){
    int error = 0;
    int *I, *J;
    double *val;
    int *row, *col;
    double *valCRS;

    int M, N, nz;
    if (argc < 2) {
        printHelp();
        exit(0);
    }
    if (readMTX(argv[1], &I, &J, &val, &M, &N, &nz) != 0)
        exit(0);
    COOtoCRS(N, nz, I, J, val, &row, &col, &valCRS);
    printf("Finished converting coordinate matrix to CRS matrix");

    free(I);
    free(J);
    free(val);
    free(row);
    free(col);
    free(valCRS);

    return 0;
}