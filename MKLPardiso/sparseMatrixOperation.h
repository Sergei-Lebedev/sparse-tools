#ifndef SPARSE_MATRIX_OPERATION
#define SPARSE_MATRIX_OPERATION

#include "type.h"
#include "error.h"
#include "routine.h"


/**
 * ���������������� ��������� � ������ �������
**/
#ifdef __cplusplus
extern "C"
#endif
int structTranspose(int n, int * column, int * row, int ** tColumn, int ** tRow);
#ifdef __cplusplus
extern "C"
#endif
int Transpose(int n, int * column, int * row, FLOAT_TYPE *val, 
              int ** tColumn, int ** tRow, FLOAT_TYPE **tVal);
/**
* API
*   sortMatrix(int n, int * column, int * row, FLOAT_TYPE* val);
*   �������������� �������
* INPUT
*   int  n                 - ������ �������
*   int * column           - CRS �������� �������
*   int * row  
*   FLOAT_TYPE *val
* OUTPUT
*   int * column           - CRS �������� �������
*   int * row  
*   FLOAT_TYPE *val
* RETURN
* ������������ ����� ������
**/
double sortMatrix(int n, int * column, int * row, FLOAT_TYPE* val);

#endif