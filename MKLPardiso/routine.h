#ifndef ROUTINE
#define ROUTINE

/**
 * ��� ������ (������ ����� ��������� ������ ...) ���
**/

#include "sparseMatrixOperation.h"
#include "error.h"
#include "type.h"

#include <stdlib.h>
#include <float.h>

#include <math.h>
#include <stdio.h>


int ReadPermutationFromFile(char* fileName, int** iperm, int n);
/**
 * API
 *   int parserSystem(int typeFile, char * fileMatrixA, char * fileVectorB, 
                      int * n, int ** column, int ** row, FLOAT_TYPE **val,
                      int * m, FLOAT_TYPE **vectorB, 
                      FLOAT_TYPE **vectorX);
 *   ������� ���������� ������� � �������. 
 *   � ���������� ������:
 *   1. ������ ������� �� �����
 *   2. ������ ������� ������ ����� �� �����
 *   3. ��������� � �������� �������� ������ ��� �������
 * INPUT
 *   int    typeFile       - ��� ������
 *                           0 - mtx
                             1 - ��������
 *   char * fileMatrixA    - ���� �������
 *   char * fileVectorB    - ���� ������ �����(������) ������� 
 * OUTPUT
 *   int * n               - ������ �������
 *   int ** column         - CRS �������� �������
 *   int ** row
 *   FLOAT_TYPE **val
 *   int * m               - ���������� ������ ������
 *   FLOAT_TYPE **vectorB  - ������(�) b
 *   FLOAT_TYPE **vectorX  - ���������� ������ �� � ������� �������
 * RETURN
 *   ������������ ��� ������
**/
int parserSystem(int typeFile, char * fileMatrixA, char * fileVectorB, 
                 int * n, int ** column, int ** row, FLOAT_TYPE **val,
                 int * m, FLOAT_TYPE **vectorB, 
                 FLOAT_TYPE **vectorX);

/**
 * API
 *   int testSolution(int n, int * column, int * row, FLOAT_TYPE *val,
                      int m, FLOAT_TYPE *vectorB, FLOAT_TYPE *vectorX,
                      FLOAT_TYPE ** calculationError);
 *   ������� �������� ������������ �������
 * INPUT
 *   int  n               - ������ �������
 *   int * column         - CRS �������� �������
 *   int * row
 *   FLOAT_TYPE *val
 *   int  m               - ���������� ������ ������
 *   FLOAT_TYPE *vectorB  - ������(�) b
 *   FLOAT_TYPE *vectorX  - ���������� ������ �� � ������� �������
 * OUTPUT
 *   FLOAT_TYPE ** calculationError - ������ ����������
 * RETURN
 *   ������������ ��� ������
**/
int testSolution(int n, int * column, int * row, FLOAT_TYPE *val,
                 int m, FLOAT_TYPE *vectorB, FLOAT_TYPE *vectorX,
                 FLOAT_TYPE ** calculationError);

/**
 * API
 *   int printSolution(char * outFile, int n, FLOAT_TYPE *vectorX);
 *     ����� ������� � ���� 
 * INPUT
 *   char * outFile
 *   int n                - ������ �������
 *   int m                - ���������� �������
 *   FLOAT_TYPE *vectorX  - ��������� �������
 * OUTPUT
 * 
 * RETURN
 *   ������������ ��� ������
**/

int printSolution(char * outFile, int n, int m, FLOAT_TYPE *vectorX);

int printSolutionBin(char *outFile, int n, int m,  FLOAT_TYPE *vectorX);

int allocMatrix(int n, int nz, int ** column, int ** row, FLOAT_TYPE **val);

int allocStructMatrix(int n, int nz, int ** column, int ** row);

/**
* API
*   int readFromFile(FILE* matrixFile, int * n, int * countNotZero, int ** columns, 
*                    int ** rows, FLOAT_TYPE ** values, int** countValueInRow,
*                    int * isOrdered, int * isStoreOnlyUpperTriangle, int * isStoreOnlyLowTriangle)
*   ������ ������� �� mtx ������� ����� � ������������ ������ �������� ������
* INPUT
*   FILE* matrixFile
* OUTPUT
*   int  n               - ������ �������
*   int * countNotZero   - ���-�� ��������� ���������
*   int ** columns       - ������� � ������������ �������
*   int ** rows
*   FLOAT_TYPE ** values
*   int** countValueInRow - ���������� �������� � ������
*   int * isOrdered       - ����������� �� �������
*   int * isStoreOnlyUpperTriangle - �������� �� ������ ������� �����������
*   int * isStoreOnlyLowTriangle   - �������� �� ������ ������ �����������
* RETURN
*   ������������ ��� ������
**/
int readMTXFile(FILE* matrixFile, int * n, int * countNotZero, int ** columns, 
                int ** rows, FLOAT_TYPE ** values, int** countValueInRow,
                int * isOrdered, int * isStoreOnlyUpperTriangle, int * isStoreOnlyLowTriangle);
/**
* API
*   checkSymmetric(int countNotZero, int * columns, int * rows, FLOAT_TYPE * values)
*   �������� ������� �� ��������������
* INPUT
*   int countNotZero
* OUTPUT
*   int * columns       - ������� � ������������ �������
*   int * rows
*   FLOAT_TYPE * values
* RETURN
*   ������������ ����������� ������� ��� ���
**/
int checkSymmetric(int countNotZero, int * columns, int * rows, FLOAT_TYPE * values);
/**
* API
*   void excisionLowTriangle(int countNotZero, int * columns, int * rows, 
*                            FLOAT_TYPE * values, int* countValueInRow, 
*                            int *countNotZeroInUpperTriangle, int *countNotZeroInLowTriangle, 
*                            int *countNotZeroInDiagonal)
*   ��������� ������� ������������
* INPUT
*   int countNotZero
* OUTPUT
*   int * columns       - ������� � ������������ �������
*   int * rows
*   FLOAT_TYPE * values
*   int* countValueInRow - ���������� �������� � ������
*   int* countNotZeroInUpperTriangle - ���-�� ��������� ��������� � ������� ������������
*   int *countNotZeroInLowTriangle - ���-�� ��������� ��������� � ������ ������������
*   int *countNotZeroInDiagonal - ���-�� ��������� ��������� �� ���������
* RETURN
**/
void excisionLowTriangle(int countNotZero, int * columns, int * rows, 
                         FLOAT_TYPE * values, int* countValueInRow, 
                         int *countNotZeroInUpperTriangle, int *countNotZeroInLowTriangle, 
                         int *countNotZeroInDiagonal);
/**
* API
*   readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
*   ������ ������� �� mtx ������� ����� � crs ������ �������� ������
* INPUT
*   char* matrixName
* OUTPUT
*   int  n               - ������ �������
*   int ** column         - CRS �������� �������
*   int ** row
*   FLOAT_TYPE **val
* RETURN
*   ������������ ��� ������
**/
int readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val);

/**
 * API
 *   int FreeMatrix(int ** column, int ** row);
 *     ������������� ������ (� ���������� ���������� ������������� NULL)
 * INPUT
 *   int ** column         - CRS �������� �������
 *   int ** row
 * OUTPUT
 *
 * RETURN
 *   ������������ ��� ������
**/
#ifdef __cplusplus
extern "C"
#endif
int freeStructMatrix(int ** column, int ** row);

/**
 * API
 *   int FreeMatrix(int ** column, int ** row, FLOAT_TYPE **val);
 *     ������������� ������ (� ���������� ���������� ������������� NULL)
 * INPUT
 *   int ** column         - CRS �������� �������
 *   int ** row
 *   FLOAT_TYPE **val
 * OUTPUT
 *
 * RETURN
 *   ������������ ��� ������
**/
int FreeMatrix(int ** column, int ** row, FLOAT_TYPE **val);
/**
 * API
 *  int freeVector(FLOAT_TYPE **vector);
 *    �������� ��������
 * INPUT
 *   FLOAT_TYPE **vector - ��������� ������
 * OUTPUT
 *
 * RETURN
 *   ������������ ��� ������
**/
int freeVector(FLOAT_TYPE **vector);
/**
 * API
 *  int allocVector(int n, FLOAT_TYPE **vector);
 *    ��������� ��������
 * INPUT
 *   int n               - ������ �������
 *   int m               - ���������� �������
 *   FLOAT_TYPE **vector - ���������� ������
 * OUTPUT
 *
 * RETURN
 *   ������������ ��� ������
**/
int allocVector(int n, int m, FLOAT_TYPE **vector);
/**
* API
*  int PrintInMTXFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
*    ������ ������� � ���� � mtx �������
* INPUT
*   char * outFile - ��� ��������� ����� 
*   int  n               - ������ �������
*   int * column         - CRS �������� �������
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
*   ������������ ��� ������
**/
int PrintInMTXFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val);
/**
* API
*  int PrintInFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
*    ������ ������� � �������� ����
* INPUT
*   char * outFile - ��� ��������� ����� 
*   int  n               - ������ �������
*   int * column         - CRS �������� �������
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
*   ������������ ��� ������
**/
int printInBinaryFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val);
/**
* API
*   readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
*   ������ ������� �� mtx ������� ����� � crs ������ �������� ������
* INPUT
*   char* matrixName
* OUTPUT
*   int  n               - ������ �������
*   int ** column         - CRS �������� �������
*   int ** row
*   FLOAT_TYPE **val
* RETURN
*   ������������ ��� ������
**/
int readMatrixFromBinaryFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val);
/**
* API
*  int printDenseMatrix(int n, int * column, int * row, FLOAT_TYPE *val)
*    ������ ������� � ������� �������
* INPUT
*   int  n               - ������ �������
*   int * column         - CRS �������� �������
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
**/
void printDenseMatrix(int n, int * column, int * row, FLOAT_TYPE *val);

  /**
* API
*   int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   ������ ������ �� MTX �����
* INPUT
*   char* fileVectorB       - ��� �����
* OUTPUT
*   int  n                  - ������ �������
*   int* m                  - ���������� ������ ������
*   FLOAT_TYPE** vectorB    - ������ ������ �����
* RETURN
*   ������������ ��� ������
**/
int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB);
/**
* API
*   int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   ������ ������ �� ��������� �����
* INPUT
*   char* fileVectorB       - ��� �����
* OUTPUT
*   int  n                  - ������ �������
*   int* m                  - ���������� ������ ������
*   FLOAT_TYPE** vectorB    - ������ ������ �����
* RETURN
*   ������������ ��� ������
**/
int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB);
/**
* API
*   ReadConfig(char* fileConfig);
*   ������ ����� ������������
* INPUT
*   char* fileConfigName - ��� ����� ������������
* OUTPUT
* RETURN
*   ������������ ��� ������
**/

#endif
