#include "routine.h"
#include "sparseMatrixOperation.h"

int allocMatrix(int n, int nz, int ** column, int ** row, FLOAT_TYPE **val)
{
    (*column) = (int        *)malloc(nz      * sizeof(int));
    (*val   ) = (FLOAT_TYPE *)malloc(nz      * sizeof(FLOAT_TYPE));
    (*row   ) = (int        *)malloc((n + 1) * sizeof(int));

    return SOLV_SYSTEM_OK;
}

int allocStructMatrix(int n, int nz, int ** column, int ** row)
{
    (*column) = (int *)malloc(nz      * sizeof(int));
    (*row   ) = (int *)malloc((n + 1) * sizeof(int));

    return SOLV_SYSTEM_OK;
}
int ReadPermutationFromFile(char* fileName, int** iperm, int n)
{
  FILE* file = fopen(fileName, "r+");
  int i;
  int *perm;
  
  (*iperm) = (int*)malloc(sizeof(int)*n);
  perm = (int*)malloc(sizeof(int)*n);


  if (!file)
  {
    fclose(file);
    return CANT_OPEN_FILE;
  }
  for (i = 0; i < n; i++)
  {
    fscanf(file, "%d\n", &(perm[i]));
  }
  for(i = 0 ; i<n;i++)
  {
	  (*iperm)[perm[i]] = i+1;
  }
  free(perm);
  fclose(file);
  return SOLV_SYSTEM_OK;
}

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
                int * isOrdered, int * isStoreOnlyUpperTriangle, int * isStoreOnlyLowTriangle)
{
    int error = SOLV_SYSTEM_OK;
    int i;
    //буферы для чтения данных из файла
    char* sBuf0 = 0;
    char* sBuf1 = 0;
    char* sBuf2 = 0;
    char* sBuf3 = 0;
    int sizeStringBuffer;
    int countUseBuffer;
    int countColumn, countRow;
    size_t charSize;
    size_t intSize;

    int currerntColumn;
    int currentRow;
    FLOAT_TYPE currentValue;

    int isFirstValueZero = 1;
    int* minColumnInRow = 0;

    charSize = sizeof(char);
    intSize = sizeof(int);
    sizeStringBuffer = 256;

    sBuf0 = (char*)malloc(sizeStringBuffer * charSize);
    sBuf1 = (char*)malloc(sizeStringBuffer * charSize);
    sBuf2 = (char*)malloc(sizeStringBuffer * charSize);
    sBuf3 = (char*)malloc(sizeStringBuffer * charSize);
    for (i = 0; i < sizeStringBuffer; i++)
    {
        sBuf0[i] = 0;
        sBuf1[i] = 0;
        sBuf2[i] = 0;
        sBuf3[i] = 0;
    }
    (*n) = -1;

    while ((sBuf0 != 0) && ((*n) <= 0))
    {
        sBuf0 = fgets(sBuf0, sizeStringBuffer, matrixFile);
        countUseBuffer = sscanf(sBuf0, "%s %s %s", sBuf1, sBuf2, sBuf3);
        countRow = atoi(sBuf1);
        countColumn = atoi(sBuf2);
        (*countNotZero) = atoi(sBuf3);
        if ((countRow > 0) && (countColumn > 0))
        {
            if (countRow !=  countColumn)
            {
                error = MATRIX_NOT_SQUARTE;
                break;
            }
            else
            {
                (*n) = countColumn;
            }
        }
    }
    if (sBuf0 == 0)
    {
        error = NO_MATRIX_IN_FILE;
    }
    if (error != 0)
    {
        return error;
    }

    (*countValueInRow) = (int*)malloc((*n) * intSize);
    minColumnInRow = (int*)malloc((*n) * intSize);


    (*columns) = (int*)malloc((*countNotZero) * intSize);
    (*rows) = (int*)malloc((*countNotZero) * intSize);
    (*values) = (FLOAT_TYPE*)malloc((*countNotZero) * sizeof(FLOAT_TYPE));

    for (i = 0; i < (*n); i++)
    {
        (*countValueInRow)[i] = 0;
        minColumnInRow[i] = 0;
    }

    //чтение файла
    for (i = 0; i < (*countNotZero); i++)
    {
        countUseBuffer = fscanf(matrixFile, "%s %s %s", sBuf0, sBuf1 , sBuf2);
        if (countUseBuffer == 3)
        {
            currentRow = atoi(sBuf0);
            currerntColumn = atoi(sBuf1);
            currentValue = atof(sBuf2);

            currentRow--;
            currerntColumn--;

            (*rows)[i] = currentRow;
            (*columns)[i] = currerntColumn;
            (*values)[i] = currentValue;

            (*countValueInRow)[currentRow]++;
            if (minColumnInRow[currentRow] >= currerntColumn)
            {
                minColumnInRow[currentRow] = currerntColumn;
                if ((*countValueInRow)[currentRow] > 1)
                {
                    (*isOrdered) = 0;
                }
            }
            if (currerntColumn < currentRow)
            {
                (*isStoreOnlyUpperTriangle) = 0;
            }

            if (currerntColumn > currentRow)
            {
                (*isStoreOnlyLowTriangle) = 0;
            }
            if ((currentRow == 0) && (currerntColumn == 0))
            {
                isFirstValueZero = 0;
            }

        }
        else
        {
            error = MISMATCH_MATRIX_FORMAT;
        }
    }

    if (isFirstValueZero == 1)
    {
        error = FIRST_VALUE_ZERO;
    }


    if (sBuf0 != 0)
    {
        free(sBuf0);
    }
    if (sBuf1 != 0)
    {
        free(sBuf1);
    }
    if (sBuf2 != 0)
    {
        free(sBuf2);
    }
    if (sBuf3 != 0)
    {
        free(sBuf3);
    }
    if (minColumnInRow != 0)
    {
        free(minColumnInRow);
    }

    return error;
}

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
int checkSymmetric(int countNotZero, int * columns, int * rows, FLOAT_TYPE * values)
{
    int i, j;
    int isHaveSymmetricValue = 1;
    int isSymmetricMatrix = 1;

    for(i = 0; i < countNotZero; i++)
    {
        isHaveSymmetricValue = 1;
        for (j = 0; j < countNotZero; j++)
        {
            if ((fabs(values[i] - values[j]) > EPSILON) && 
                (columns[i] == rows[j]) && 
                (rows[i] == columns[j]) && 
                (i != j))
            {
                isHaveSymmetricValue = 0;
                break;
            }
        }
        if(!isHaveSymmetricValue)
        {
            isSymmetricMatrix = 0;
            break;
        }
    }

    return isSymmetricMatrix;
}

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
                         int *countNotZeroInDiagonal)
{
    int i;

    for(i = 0; i < countNotZero; i++)
    {
        if ((columns[i] < rows[i]) && (rows[i] != -1))
        {
            countValueInRow[rows[i]]--;
            columns[i] = -1;
            rows[i] = -1;
            values[i] = 0;
            (*countNotZeroInLowTriangle)++;
        }
        else
            if (columns[i] > rows[i])
            {
                (*countNotZeroInUpperTriangle)++;
            }
            else
                if (columns[i] == rows[i])
                {
                    (*countNotZeroInDiagonal)++;
                }
    }
}

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
int readMatrixFromFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
{
    int i;
    int countNotZero;//число не нулевых элементов в строке
    int countNotZeroOld;
    size_t intSize;
    FILE* matrixFile  = 0;   
    int error;
    int* countValueInRow = 0;
    int* currentIndexInRow = 0;
    int* columns = 0;
    int* rows = 0;
    FLOAT_TYPE* values = 0; 
    int isSymmetricMatrix;
    int isStoreOnlyUpperTriangle;
    int isStoreOnlyLowTriangle;
    int isOrdered;
    int transposeBuffer;
    int countNotZeroInUpperTriangle, countNotZeroInLowTriangle ,countNotZeroInDiagonal;
    int isDiagonalZero;

    isSymmetricMatrix = 1;
    isStoreOnlyUpperTriangle = 1;
    isStoreOnlyLowTriangle = 1;
    isOrdered = 1;
    transposeBuffer = 0;

    countNotZero = -1;
    isDiagonalZero = 0;

    intSize = sizeof(int);
    error = SOLV_SYSTEM_OK;

    matrixFile = fopen(matrixName, "r");
    if(matrixFile == 0)
    {
        printf("Error open files %s\n", matrixName);
        return CANT_OPEN_FILE;
    }

    //читаем файл
    error = readMTXFile(matrixFile, n, &countNotZero, &columns, &rows, &values, 
        &countValueInRow, &isOrdered, &isStoreOnlyUpperTriangle, &isStoreOnlyLowTriangle);

    if (error != 0)
    {
        fclose(matrixFile);
        return error;
    }

    countNotZeroOld = countNotZero;
    currentIndexInRow = (int*)malloc((*n) * intSize);
    for (i = 0; i < (*n); i++)
    {
        currentIndexInRow[i] = 0;
    }

    //если матрица задана нижним треугольником, то транспонируем
    if (isStoreOnlyLowTriangle == 1)
    {
        isStoreOnlyLowTriangle = 0;
        isStoreOnlyUpperTriangle = 1;
        isOrdered = 0;
        for (i = 0; i < (*n); i++)
        {
            countValueInRow[i] = 0;
        }
        for(i = 0; i < countNotZero; i++)
        {
            transposeBuffer = rows[i];
            rows[i] = columns[i];
            columns[i] = transposeBuffer;
            countValueInRow[rows[i]]++;
        }
    }

    //если матрица хранит не только верхний треугольник и диагональ, то проверяем на симметричность
    if (!isStoreOnlyUpperTriangle)
    {
        isSymmetricMatrix = checkSymmetric(countNotZero, columns, rows, values);

        countNotZeroInUpperTriangle = 0;
        countNotZeroInLowTriangle = 0;
        countNotZeroInDiagonal = 0;        

        //если матрица симметричная отрезаем нижний треугольник матрицы
        if (isSymmetricMatrix == 1)
        {
            excisionLowTriangle(countNotZero, columns, rows, values, countValueInRow, 
                &countNotZeroInUpperTriangle, &countNotZeroInLowTriangle, &countNotZeroInDiagonal);
            isOrdered = 0;
            countNotZero = countNotZero - countNotZeroInLowTriangle;
        }
    }

    if (isSymmetricMatrix == 0)
    {
        error = MATRIX_NOT_SYMMETRIC;
    }

    /*for (i = 0; i < (*n); i++)
    {
        if (countValueInRow[i] == 0)
        {
            countValueInRow[i] = 1;
            isOrdered = 0;
            countNotZero++;
            isDiagonalZero = 1;
        }
    }*/  

    /*if (countNotZeroInDiagonal != (*n))
    {
        error = FIRST_VALUE_ZERO;
    }*/

    //выделение памяти под матрицу
    allocMatrix((*n), countNotZero, column, row, val);
    for (i = 0; i < countNotZero; i++)
    {
        (*column)[i] = -1;
        (*val)[i] = -1;
    }
    for (i = 0; i <= (*n); i++)
    {
        (*row)[i] = -1;
    }

    (*row)[0] = 0;
    //заполнение индексов начала строк матрицы
    for (i = 0; i < (*n); i++)
    {
        (*row)[i + 1] = (*row)[i] + countValueInRow[i];
    }

    //заполнение номера колонки и значений матрицы
    for (i = 0; i < countNotZeroOld; i++)
    {
        if (rows[i] != -1)
        {
            (*column)[(*row)[rows[i]] + currentIndexInRow[rows[i]]] = columns[i];
            (*val)[(*row)[rows[i]] + currentIndexInRow[rows[i]]] = values[i];
            currentIndexInRow[rows[i]]++;        
        }
    }

    if (isDiagonalZero == 1)
    {
        for (i = 0; i < (*n); i++)
        {
            if((*column)[(*row)[i]] == -1)
            {
                (*column)[(*row)[i]] = i;
                (*val)[(*row)[i]] = 0;
            }
        }
    }

    //если матрица не упорядоченная, то упорядочиваем
    if (!isOrdered)
    {
        sortMatrix(*n, *column, *row, *val);
    }
    if (countValueInRow != 0)
    {
        free(countValueInRow);
    }
    if (currentIndexInRow != 0)
    {
        free(currentIndexInRow);
    }
    if (columns != 0)
    {
        free(columns);
    }
    if (rows != 0)
    {
        free(rows);
    }
    if (values != 0)
    {
        free(values);
    }
    if (matrixFile != 0)
    {
        fclose(matrixFile);
    }
    return error;
}

int ReadMatrixFromBinarySFile(char* fileMatrixA, 
                              int* n, int** column, int** row,
                              FLOAT_TYPE** val)
{
 
  return 0;
}

int ReadMatrixFromBinaryBlockFile(char* fileMatrixA, 
                              int* n, int** column, int** row,
                              FLOAT_TYPE** val, int *block_size)
{

  return 0;
}

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
                 FLOAT_TYPE **vectorX)
{
    int i = 0;
    int j =0, s,f;
    int block_size = 1;
    int error = 0;

    int fDiag;
    int fU;
printf("switch\n");
    switch (typeFile)
    {
    case 0:
        {
            readMatrixFromFile(fileMatrixA, n, column, row, val);
            break;
        }
    case 1:
        {   

            readMatrixFromBinaryFile(fileMatrixA, n, column, row, val);
           
break;
        }
    case 3:
    {
            ReadMatrixFromBinaryBlockFile(fileMatrixA, n, column, row, val, &block_size);
            break;
    }
    default :
        {
            *n      = 0;
            *column = NULL;
            *row    = NULL;
            *val    = NULL;
        }
    }

    fDiag = 1;
    fU    = 1;
    for(i = 0; i < (*n); i++)
    {
        s = (*row)[i];
        f = (*row)[i + 1];
        if((*column)[s] != i)
        {
            fDiag = 0;
            //printf("bag 1");
        }
        for(j = s; j < f; j++)
        {
            if((*column)[s] < i)
            {
                fU = 0;
                //printf("bag 2");
            }
        }
    }

    if(fDiag == 0)
    {
        printf("miss diagonal element\n");
    }
    if(fU == 0)
    {
        printf("matrix is not U\n");
    }

      switch (typeFile)
  {
  case 0:
    {
      error = ReadVectorMTXFile(fileVectorB, *n, m, vectorB);
      break;
    }
  case 1:
    {
      if (strcmp(fileVectorB, "qqq.bin") != 0)
		error = ReadVectorBinFile(fileVectorB, *n, m, vectorB);
	  else
		error = ReadVectorMTXFile(fileVectorB, *n, m, vectorB);
	  break;
    }
  case 2:
    {
      error = ReadVectorMTXFile(fileVectorB, *n, m, vectorB);
      break;
    }
  case 3:
    {
      error = ReadVectorBinFile(fileVectorB, *n, m, vectorB);
      break;
    }
  default :
    {
          (*m)       = 1;
    (*vectorB) = (FLOAT_TYPE *)malloc (((*n) * (*m)) * sizeof(FLOAT_TYPE));
    for(i = 0; i < (*n) * (*m); i++)
    {
        (*vectorB)[i] = (FLOAT_TYPE)1.0;
    }           
    }
  }



    (*vectorX) = (FLOAT_TYPE *)malloc (((*n) * (*m)) * sizeof(FLOAT_TYPE));

    return SOLV_SYSTEM_OK;
}

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
                 FLOAT_TYPE ** calculationError)
{
    int i, j, k, s, f;
    int isFloat;
    int nz;
    int statusFloatVal;

    FLOAT_TYPE *calcB;
    int *tCol;
    int *tRow;
    FLOAT_TYPE *tVal;

    nz = row[n];
    isFloat = 1;

    if(isFloat == 0)
    {
        printf("check error: (NaN)\n");
        return NO_FLOAT_VALUE;
    }

    calcB               = (FLOAT_TYPE *)malloc ( (n * m) * sizeof(FLOAT_TYPE));
    (*calculationError) = (FLOAT_TYPE *)malloc ( m       * sizeof(FLOAT_TYPE));

    for(i = 0; i < m; i++)
    {
        (*calculationError)[i] = (FLOAT_TYPE)0.0;
    }
    for(i = 0; i < m * n; i++)
    {
        calcB[i] = (FLOAT_TYPE)0.0;
    }

    for(j = 0; j < m; j++)
    {
        for(i = 0; i < n; i++)
        {
            s = row[i];
            f = row[i + 1];
            for(k = s; k < f; k++)
            {
                calcB[j * n + i] += val[k] * vectorX[j * n + column[k]];
            }
        }
    }

    Transpose(n, column, row, val, &tCol, &tRow, &tVal);
    for(j = 0; j < m; j++)
    {
        for(i = 0; i < n; i++)
        {
            s = tRow[i];
            f = tRow[i + 1] - 1;
            for(k = s; k < f; k++)
            {
                calcB[j * n + i] += tVal[k] * vectorX[j * n + tCol[k]];
            }
        }
    }

    for(j = 0; j < m; j++)
    {
        for(i = 0; i < n; i++)
        {
            if( (*calculationError)[j] < 
                (calcB[j * n + i] - vectorB[j * n + i]) * 
                (calcB[j * n + i] - vectorB[j * n + i])   )
            {
                (*calculationError)[j] = 
                    (calcB[j * n + i] - vectorB[j * n + i]) * 
                    (calcB[j * n + i] - vectorB[j * n + i]);
            }
        }
    }

    FreeMatrix(&tCol, &tRow, &tVal);

    free(calcB);
    return SOLV_SYSTEM_OK;
}
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
int printSolution(char * outFile, int n, int m, FLOAT_TYPE *vectorX)
{
    FILE* vectorFile;
    int i = 0;
    int j = 0;

    vectorFile = fopen(outFile, "w");

    if (vectorFile == 0)
    {
        printf("Error open files %s\n", outFile);
        return CANT_OPEN_FILE;
    }

    fprintf(vectorFile, "%d %d \n", n, m);
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            fprintf(vectorFile, "%f \n", vectorX[i * n + j]);
    }

    fclose(vectorFile);
    return SOLV_SYSTEM_OK;
}


/**
* API
*   int freeStructMatrix(int ** column, int ** row)
*     высвобождение памяти (в результате указателям присваивается NULL)
* INPUT
*   int ** column         - CRS описание матрицы
*   int ** row
* OUTPUT
*
* RETURN
*   возвращается код ошибки
**/
int freeStructMatrix(int ** column, int ** row)
{
    free(*column);
    free(*row);
    (*column) = NULL;
    (*row)    = NULL;
    return SOLV_SYSTEM_OK;
}

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
int FreeMatrix(int ** column, int ** row, FLOAT_TYPE **val)
{
    free(*column);
    free(*row);
    free(*val);
    (*column) = NULL;
    (*row)    = NULL;
    (*val)    = NULL;
    return SOLV_SYSTEM_OK;
}

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
int freeVector(FLOAT_TYPE **vector)
{
    free(*vector);
    (*vector) = NULL;
    return SOLV_SYSTEM_OK;
}

/**
* API
*  int allocVector(int n, FLOAT_TYPE **vector);
*    выделение векторов
* INPUT
*   FLOAT_TYPE **vector - выделяемый вектор
* OUTPUT
*
* RETURN
*   возвращается код ошибки
**/
int allocVector(int n, int m, FLOAT_TYPE **vector)
{
    (*vector) = (FLOAT_TYPE *)malloc(n * m * sizeof(FLOAT_TYPE));
    return SOLV_SYSTEM_OK;
}

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
int PrintInMTXFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
{
    FILE* matrixFile;
    int rowStart = 0;
    int rowEnd = 0;
    int j = 0;
    int k = 0;

    matrixFile = fopen(outFile, "w");

    if (matrixFile == 0)
    {
        printf("Error open files %s\n", outFile);
        return CANT_OPEN_FILE;
    }

    fprintf(matrixFile, "%d %d %d \n", n, n, row[n]);
    for (j = 0; j < n; j++)
    {
        rowStart = row[j];
        rowEnd = row[j + 1] - 1;
        for (k = rowStart; k <= rowEnd; k++)
            fprintf(matrixFile, "%d %d %f \n", j + 1, column[k] + 1, val[k]);
    }

    fclose(matrixFile);
    return SOLV_SYSTEM_OK;
}
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
int printInBinaryFile(char * outFile, int n, int * column, int * row, FLOAT_TYPE *val)
{
    FILE* matrixFile;

    matrixFile = fopen(outFile, "w");
    if (matrixFile == 0)
    {
        printf("Error open files %s\n", outFile);
        return CANT_OPEN_FILE;
    }
    fwrite(&n, sizeof(int), 1, matrixFile);
    fwrite(&row[n], sizeof(int), 1, matrixFile);
    fwrite(column, sizeof(int), row[n], matrixFile);
    fwrite(row, sizeof(int), n + 1, matrixFile);
    fwrite(val, sizeof(FLOAT_TYPE), row[n], matrixFile);

    fclose(matrixFile);
    return SOLV_SYSTEM_OK;
}
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
int readMatrixFromBinaryFile(char* matrixName, int * n, int ** column, int ** row, FLOAT_TYPE **val)
{

  FILE* matrixFile  = 0;   
  int countRead = 0;
  int countNotZero = 0;

  int error = SOLV_SYSTEM_OK;

  matrixFile = fopen(matrixName, "rb");

  if(matrixFile == 0)
  {
    printf("Error open files %s\n", matrixName);
    return CANT_OPEN_FILE;
  }

  countRead = fread(n, sizeof(int), 1, matrixFile);
  if (countRead != 1)
  {
    error = CANT_OPEN_FILE;
  }
  countRead = fread(&countNotZero, sizeof(int), 1, matrixFile);
  if (countRead != 1)
  {
    error = CANT_OPEN_FILE;
  }

    allocMatrix((*n), countNotZero, column, row, val);

  countRead = fread(*column, sizeof(int), countNotZero, matrixFile);

  countRead = fread(*row, sizeof(int), (*n) + 1, matrixFile);
  if (countRead != (*n) + 1)
  {
    error = CANT_OPEN_FILE;
  }
  countRead = fread(*val, sizeof(FLOAT_TYPE), countNotZero, matrixFile);
  if (countRead != (*row)[(*n)])
  {
    error = CANT_OPEN_FILE;
  }

  fclose(matrixFile);
  return error;

}

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
void printDenseMatrix(int n, int * column, int * row, FLOAT_TYPE *val)
{
    double* m1 = (double* )malloc(n * n * sizeof(double));
    int i;
    int j;

    memset(m1, 0, n * n * sizeof(double));
    for(i = 0; i < n; i++)
    {
        for(j = row[i]; j < row[i + 1]; j++)
        {
            m1[i * n + column[j]] = val[j];
        }
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("\t%.1f", m1[i * n + j]);
        }
        printf("\n");
    }
    printf("\n");
    free(m1);
}

/**
* API
*   int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   чтение вектор из MTX файла
* INPUT
*   char* fileVectorB   - имя файла
* OUTPUT
*   int  n               - размер матрицы
*   int* m              - количество правых частей
*   FLOAT_TYPE** vectorB - вектор правой части
* RETURN
*   возвращается код ошибки
**/
int ReadVectorMTXFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
{
  int i;

  (*m)       = 1;
  (*vectorB) = (FLOAT_TYPE *)malloc ((n * (*m)) * sizeof(FLOAT_TYPE));
  for(i = 0; i < n * (*m); i++)
  {
    (*vectorB)[i] = (FLOAT_TYPE)1.0;    
  }                                     

  return SOLV_SYSTEM_OK;
}
/**
* API
*   int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
*   чтение вектор из бинарного файла
* INPUT
*   char* fileVectorB   - имя файла
* OUTPUT
*   int  n               - размер матрицы
*   int* m              - количество правых частей
*   FLOAT_TYPE** vectorB - вектор правой части
* RETURN
*   возвращается код ошибки
**/
int ReadVectorBinFile(char* fileVectorB, int n, int* m, FLOAT_TYPE** vectorB)
{
	int i, countRead;
	int error=0;
	FILE* rightHandFile  = 0;

	(*m)       = 1;
	*vectorB = (FLOAT_TYPE*)malloc((n * (*m)) * sizeof(FLOAT_TYPE));

	rightHandFile = fopen(fileVectorB, "rb");
	if(rightHandFile == 0)
	{
		printf("Error open file %s\n", fileVectorB);
		error = CANT_OPEN_FILE;
	}
	fseek(rightHandFile, 20, SEEK_SET); //for Sarov right hand side storage format
	countRead = fread(*vectorB, sizeof(FLOAT_TYPE), n, rightHandFile);
	if (countRead != n)
	{
		printf("Error reading file %s\n", fileVectorB);
		error = CANT_OPEN_FILE;
	}
	return error;
  return 0;
}