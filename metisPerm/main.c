#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <metis.h>

void showHelp(){
    printf("Program to get metis permutation for symmetric matrix\n");
    printf("First argument - matrix in binary format\n");
    printf("Second argument - file to save permuatation\n");
}

int readBinCRSstruct(char *filename, int *n, int **row, int **col){
    FILE *f;
    int nz;
    f = fopen(filename, "rb");
    fread(n, sizeof(int), 1, f);
    fread(&nz, sizeof(int), 1, f);

    (*row) = (int*)malloc((*n+1) * sizeof(int));
    (*col) = (int*)malloc(nz * sizeof(int));

    fread(*col, sizeof(int), nz, f);
    fread(*row, sizeof(int), *n+1, f);

    fclose(f);
    return 0;
}

int CRSMatrixToGraph(int* column, int* row, int** GraphXadj, int** GraphAdjncy, int n) {
    int i;  int j;  int neigb;
    int nEdges = 2*(row[n] - n);  int* AdjPos;
    (*GraphXadj) = (int*)malloc(sizeof(int)*(n + 1));
    (*GraphAdjncy) = (int*)malloc(sizeof(int)*nEdges);
    AdjPos = (int*)malloc(sizeof(int)*n);
    memset(AdjPos, 0, sizeof(int)*n);
    for (i = 0; i < n; i++)
    {
        AdjPos[i] += row[i + 1] - row[i] - 1;
        for (j = row[i] + 1; j < row[i + 1]; j++)
            AdjPos[column[j]]++;  } (*GraphXadj)[0] = 0;
    for(i = 1; i < n + 1; i++)
        (*GraphXadj)[i] = (*GraphXadj)[i - 1] + AdjPos[i - 1];
    memcpy(AdjPos, (*GraphXadj), sizeof(int)*n);
    for (i = 0; i < n; i++)
    {
        for(j = row[i] + 1; j < row[i + 1]; j++)
        {
            (*GraphAdjncy)[AdjPos[i]] = column[j];
            neigb = column[j];
            (*GraphAdjncy)[AdjPos[neigb]] = i;
            AdjPos[i]++; AdjPos[neigb]++;
        }
    }
    free(AdjPos);
    return 0;
}

int getMetisPermutation(int *column, int *row, int *perm, int *iperm, int n) {
    int* GraphXadj;	int* GraphAdjncy;
    //not working on Windows 10 + VS2015 + Intel compiler 2016
    int* options = (int*)malloc(sizeof(int)*METIS_NOPTIONS);

    METIS_SetDefaultOptions(options);
    CRSMatrixToGraph(column, row, &GraphXadj, &GraphAdjncy, n);
    METIS_NodeND(&n, GraphXadj, GraphAdjncy, NULL, options, perm, iperm);
    free(GraphXadj);
    free(GraphAdjncy);
    free(options);
    return 0;
}

int savePermutation(char *filename, int n, int *iperm){
    FILE *f;
    int i;
    f = fopen(filename, "w");
    for(i = 0; i < n; i++)
        fprintf(f, "%d\n", iperm[i]);
    fclose(f);
    return 0;
}
int main(int argc, char* argv[]){
    int *row, *col, n;
    int *perm, *iperm;
    if (argc < 3) {
        showHelp();
        return 0;
    }
    readBinCRSstruct(argv[1], &n, &row, &col);
    perm = (int*)malloc(n * sizeof(int));
    iperm = (int*)malloc(n * sizeof(int));
    getMetisPermutation(col, row, perm, iperm, n);
    savePermutation(argv[2], n, iperm);
    free(row);
    free(perm);
    free(iperm);
    free(col);
    return 0;
}