#ifndef ROUTINE
#define ROUTINE

/**
 * вся рктина (чтение вывод выделение памяти ...) тут
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
 *   функция подготовки системы к решению. 
 *   В подготовку входит:
 *   1. чтение матрицы из файла
 *   2. чтение вектора правой части из файла
 *   3. выделение и возможно заполние памяти под решение
 * INPUT
 *   int    typeFile       - тип файлов
 *                           0 - mtx
                             1 - бинарный
 *   char * fileMatrixA    - файл матрицы
 *   char * fileVectorB    - файл правой части(частей) матрицы 
 * OUTPUT
 *   int * n               - размер матрицы
 *   int ** column         - CRS описание матрицы
 *   int ** row
 *   FLOAT_TYPE **val
 *   int * m               - количество правых частей
 *   FLOAT_TYPE **vectorB  - вектор(ы) b
 *   FLOAT_TYPE **vectorX  - выделенная память по д решение системы
 * RETURN
 *   возвращается код ошибки
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
 *   функция проверки правильности решения
 * INPUT
 *   int  n               - размер матрицы
 *   int * column         - CRS описание матрицы
 *   int * row
 *   FLOAT_TYPE *val
 *   int  m               - количество правых частей
 *   FLOAT_TYPE *vectorB  - вектор(ы) b
 *   FLOAT_TYPE *vectorX  - выделенная память по д решение системы
 * OUTPUT
 *   FLOAT_TYPE ** calculationError - ошибки вычислений
 * RETURN
 *   возвращается код ошибки
**/
int testSolution(int n, int * column, int * row, FLOAT_TYPE *val,
                 int m, FLOAT_TYPE *vectorB, FLOAT_TYPE *vectorX,
                 FLOAT_TYPE ** calculationError);

/**
 * API
 *   int printSolution(char * outFile, int n, FLOAT_TYPE *vectorX);
 *     вывод решения в файл 
 * INPUT
 *   char * outFile
 *   int n                - размер матрицы
 *   int m                - количество решений
 *   FLOAT_TYPE *vectorX  - выводимые решения
 * OUTPUT
 * 
 * RETURN
 *   возвращается код ошибки
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
*   чтение матрицы из mtx формата файла в координатный формат хранения матриц
* INPUT
*   FILE* matrixFile
* OUTPUT
*   int  n               - размер матрицы
*   int * countNotZero   - кол-во ненулевых элементов
*   int ** columns       - матрица в координатном формате
*   int ** rows
*   FLOAT_TYPE ** values
*   int** countValueInRow - количество значений в строке
*   int * isOrdered       - упорядочена ли матрица
*   int * isStoreOnlyUpperTriangle - хранится ли только верхний треугольник
*   int * isStoreOnlyLowTriangle   - хранится ли только нижний треугольник
* RETURN
*   возвращается код ошибки
**/
int readMTXFile(FILE* matrixFile, int * n, int * countNotZero, int ** columns, 
                int ** rows, FLOAT_TYPE ** values, int** countValueInRow,
                int * isOrdered, int * isStoreOnlyUpperTriangle, int * isStoreOnlyLowTriangle);
/**
* API
*   checkSymmetric(int countNotZero, int * columns, int * rows, FLOAT_TYPE * values)
*   проверка матрицы на симметричность
* INPUT
*   int countNotZero
* OUTPUT
*   int * columns       - матрица в координатном формате
*   int * rows
*   FLOAT_TYPE * values
* RETURN
*   возвращается симметрична матрица или нет
**/
int checkSymmetric(int countNotZero, int * columns, int * rows, FLOAT_TYPE * values);
/**
* API
*   void excisionLowTriangle(int countNotZero, int * columns, int * rows, 
*                            FLOAT_TYPE * values, int* countValueInRow, 
*                            int *countNotZeroInUpperTriangle, int *countNotZeroInLowTriangle, 
*                            int *countNotZeroInDiagonal)
*   отрезание нижнего треугольника
* INPUT
*   int countNotZero
* OUTPUT
*   int * columns       - матрица в координатном формате
*   int * rows
*   FLOAT_TYPE * values
*   int* countValueInRow - количество значений в строке
*   int* countNotZeroInUpperTriangle - кол-во ненулевых элементов в верхнем треугольнике
*   int *countNotZeroInLowTriangle - кол-во ненулевых элементов в нижнем треугольнике
*   int *countNotZeroInDiagonal - кол-во ненулевых элементов на диагонале
* RETURN
**/
void excisionLowTriangle(int countNotZero, int * columns, int * rows, 
                         FLOAT_TYPE * values, int* countValueInRow, 
                         int *countNotZeroInUpperTriangle, int *countNotZeroInLowTriangle, 
                         int *countNotZeroInDiagonal);
/**
* API
*   readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
*   чтение матрицы из mtx формата файла в crs формат хранения матриц
* INPUT
*   char* matrixName
* OUTPUT
*   int  n               - размер матрицы
*   int ** column         - CRS описание матрицы
*   int ** row
*   FLOAT_TYPE **val
* RETURN
*   возвращается код ошибки
**/
int readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val);

/**
 * API
 *   int FreeMatrix(int ** column, int ** row);
 *     высвобождение памяти (в результате указателям присваивается NULL)
 * INPUT
 *   int ** column         - CRS описание матрицы
 *   int ** row
 * OUTPUT
 *
 * RETURN
 *   возвращается код ошибки
**/
#ifdef __cplusplus
extern "C"
#endif
int freeStructMatrix(int ** column, int ** row);

/**
 * API
 *   int FreeMatrix(int ** column, int ** row, FLOAT_TYPE **val);
 *     высвобождение памяти (в результате указателям присваивается NULL)
 * INPUT
 *   int ** column         - CRS описание матрицы
 *   int ** row
 *   FLOAT_TYPE **val
 * OUTPUT
 *
 * RETURN
 *   возвращается код ошибки
**/
int FreeMatrix(int ** column, int ** row, FLOAT_TYPE **val);
/**
 * API
 *  int freeVector(FLOAT_TYPE **vector);
 *    удаление векторов
 * INPUT
 *   FLOAT_TYPE **vector - удаляемый вектор
 * OUTPUT
 *
 * RETURN
 *   возвращается код ошибки
**/
int freeVector(FLOAT_TYPE **vector);
/**
 * API
 *  int allocVector(int n, FLOAT_TYPE **vector);
 *    выделение векторов
 * INPUT
 *   int n               - размер матрицы
 *   int m               - количество решений
 *   FLOAT_TYPE **vector - выделяемый вектор
 * OUTPUT
 *
 * RETURN
 *   возвращается код ошибки
**/
int allocVector(int n, int m, FLOAT_TYPE **vector);
/**
* API
*  int PrintInMTXFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
*    печать матрицы в файл в mtx формате
* INPUT
*   char * outFile - имя выходного файла 
*   int  n               - размер матрицы
*   int * column         - CRS описание матрицы
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
*   возвращается код ошибки
**/
int PrintInMTXFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val);
/**
* API
*  int PrintInFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
*    печать матрицы в бинарный файл
* INPUT
*   char * outFile - имя выходного файла 
*   int  n               - размер матрицы
*   int * column         - CRS описание матрицы
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
*   возвращается код ошибки
**/
int printInBinaryFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val);
/**
* API
*   readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
*   чтение матрицы из mtx формата файла в crs формат хранения матриц
* INPUT
*   char* matrixName
* OUTPUT
*   int  n               - размер матрицы
*   int ** column         - CRS описание матрицы
*   int ** row
*   FLOAT_TYPE **val
* RETURN
*   возвращается код ошибки
**/
int readMatrixFromBinaryFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val);
/**
* API
*  int printDenseMatrix(int n, int * column, int * row, FLOAT_TYPE *val)
*    печать матрицы в плотном формате
* INPUT
*   int  n               - размер матрицы
*   int * column         - CRS описание матрицы
*   int * row
*   FLOAT_TYPE *val
* OUTPUT
* RETURN
**/
void printDenseMatrix(int n, int * column, int * row, FLOAT_TYPE *val);

  /**
* API
*   int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   чтение вектор из MTX файла
* INPUT
*   char* fileVectorB       - имя файла
* OUTPUT
*   int  n                  - размер матрицы
*   int* m                  - количество правых частей
*   FLOAT_TYPE** vectorB    - вектор правой части
* RETURN
*   возвращается код ошибки
**/
int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB);
/**
* API
*   int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   чтение вектор из бинарного файла
* INPUT
*   char* fileVectorB       - имя файла
* OUTPUT
*   int  n                  - размер матрицы
*   int* m                  - количество правых частей
*   FLOAT_TYPE** vectorB    - вектор правой части
* RETURN
*   возвращается код ошибки
**/
int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB);
/**
* API
*   ReadConfig(char* fileConfig);
*   чтение файла конфигураций
* INPUT
*   char* fileConfigName - имя файла конфигурации
* OUTPUT
* RETURN
*   возвращается код ошибки
**/

#endif
