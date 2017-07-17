#include "sparseMatrixOperation.h"

#include <time.h>

int structTranspose(int n, int * column, int * row, int ** tColumn, int ** tRow)
{
	int i, j;
    int nz;
    int S;
    nz = row[n];

    allocStructMatrix(n, nz, tColumn, tRow);

	
	memset((*tRow), 0, (n + 1) * sizeof(int));
	for (i = 0; i < nz; i++) 
    {
		(*tRow)[column[i] + 1]++;
    }

	S = 0;
	for (i = 1; i <= n; i++) 
	{
		int tmp = (*tRow)[i];
		(*tRow)[i] = S;
		S = S + tmp;
	}

	for (i = 0; i < n; i++) 
	{
		int j1 = row[i];
		int j2 = row[i+1];
		int Col = i; // Столбец в AT - строка в А
		for (j = j1; j < j2; j++) 
		{
			int RIndex = column[j];  // Строка в AT
			int IIndex = (*tRow)[RIndex + 1];
			(*tColumn)[IIndex] = Col;
			(*tRow)   [RIndex + 1]++;
		}
	}
    return SOLV_SYSTEM_OK;
}

int Transpose(int n, int * column, int * row, FLOAT_TYPE *val, 
              int ** tColumn, int ** tRow, FLOAT_TYPE **tVal)
{
  int i, j, nz;
  int S;

  nz = row[n];

  allocMatrix(n, nz, tColumn, tRow, tVal);
  
  memset((*tRow), 0, (n + 1) * sizeof(int));
  for (i = 0; i < nz; i++) 
    (*tRow)[column[i] + 1]++;
  
  S = 0;
  for (i = 1; i <= n; i++) 
  {
    int tmp = (*tRow)[i];
    (*tRow)[i] = S;
    S = S + tmp;
  }

  for (i = 0; i < n; i++) 
  {
    int j1 = row[i];
    int j2 = row[i+1];
    int Col = i; // Столбец в AT - строка в А
    for (j = j1; j < j2; j++) 
    {
      double V = val[j];  // Значение
      int RIndex = column[j];  // Строка в AT
      int IIndex = (*tRow)[RIndex + 1];
      (*tVal)   [IIndex] = V;
      (*tColumn)[IIndex] = Col;
      (*tRow)   [RIndex + 1]++;
    }
  }

  return SOLV_SYSTEM_OK;
}
/**
* API
*   sortMatrix(int n, int * column, int * row, FLOAT_TYPE* val);
*   Упорядочивание матрицы
* INPUT
*   int  n                 - размер матрицы
*   int * column           - CRS описание матрицы
*   int * row  
*   FLOAT_TYPE *val
* OUTPUT
*   int * column           - CRS описание матрицы
*   int * row  
*   FLOAT_TYPE *val
* RETURN
* возвращается время работы
**/
double sortMatrix(int n, int * column, int * row, FLOAT_TYPE* val)
{
    clock_t start, finish;
    int i, j;
    int nz;
    int s;
    int j1; 
    int j2; 
    int col;
    double V;
    int RIndex;
    int IIndex;
    int tmp;
    int * tColumn; 
    int * tRow; 
    FLOAT_TYPE* tVal;
    nz = row[n];
    s = 0;

    start = clock();

    allocMatrix(n, nz, &tColumn, &tRow, &tVal);

    memset(tRow, 0, (n + 1) * sizeof(int));
    for (i = 0; i < nz; i++) 
        tRow[column[i] + 1]++;

    for (i = 1; i <= n; i++) 
    {
        tmp = tRow[i];
        tRow[i] = s;
        s = s + tmp;
    }

    for (i = 0; i < n; i++) 
    {
        j1 = row[i];
        j2 = row[i+1];
        col = i; // Столбец в AT - строка в А
        for (j = j1; j < j2; j++) 
        {
            V = val[j];  // Значение
            RIndex = column[j];  // Строка в AT
            IIndex = tRow[RIndex + 1];
            tVal[IIndex] = V;
            tColumn  [IIndex] = col;
            tRow[RIndex + 1]++;
        }
    }

    memset(row, 0, (n + 1) * sizeof(int));
    s = 0;
    for (i = 0; i < nz; i++) 
        row[tColumn[i] + 1]++;

    for (i = 1; i <= n; i++) 
    {
        tmp = row[i];
        row[i] = s;
        s = s + tmp;
    }

    for (i = 0; i < n; i++) 
    {
        j1 = tRow[i];
        j2 = tRow[i+1];
        col = i; // Столбец в AT - строка в А
        for (j = j1; j < j2; j++) 
        {
            V = tVal[j];  // Значение
            RIndex = tColumn[j];  // Строка в AT
            IIndex = row[RIndex + 1];
            val[IIndex] = V;
            column  [IIndex] = col;
            row[RIndex + 1]++;
        }
    }    

    FreeMatrix(&tColumn, &tRow, &tVal);

    finish = clock();
    return ((double)(finish - start)) / CLOCKS_PER_SEC;
}