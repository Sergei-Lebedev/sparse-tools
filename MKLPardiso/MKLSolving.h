#ifndef MKLSolving
#define MKLSolving

#include "sparseMatrixOperation.h"
#include "error.h"
#include "type.h"

#include <stdlib.h>

void MKL_SparseSolving(int n, int m, int * column, int * row, FLOAT_TYPE *val, FLOAT_TYPE *b, FLOAT_TYPE* resX, int *iPerm);
//определение детерминанта с помощью mkl
double MKL_Determinant(int n, int m, int * column, int * row, FLOAT_TYPE *val,FLOAT_TYPE *b, FLOAT_TYPE* resX);
#endif