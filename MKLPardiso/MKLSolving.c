#include "mkl_dss.h"
//#include "mkl_spblas.h"

#include "mkl_pardiso.h"

#include "MKLSolving.h"

#include "mkl_dss.h"
#include "mkl.h"


#if defined(_WIN32) || defined(_WIN64)
#define pardiso_ PARDISO
#else
#define PARDISO pardiso_
#endif
#if defined(MKL_ILP64)
#define MKL_INT long long
#else
#define MKL_INT int
#endif

//решение системы слинейных уравнений
//с использованием MKL PARDISO
void MKL_SparseSolving(int n, int m, int * column, int * row, FLOAT_TYPE *val, FLOAT_TYPE *b, FLOAT_TYPE* resX, int* iPerm)
{
  MKL_INT i;
  int nz = row[n];
  //указатель на "решатель"
  _MKL_DSS_HANDLE_t handle;

  MKL_INT mtype = 2; /* матрица симметричная и положительно определенная */
  /* RHS and solution vectors. */
  MKL_INT nrhs = m; /* число правых частей */
  /* Internal solver memory pointer pt, */
  /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
  /* or void *pt[64] should be OK on both architectures */
  void *pt[64];
  /* Pardiso control parameters. */
  MKL_INT iparm[64];
  MKL_INT maxfct, mnum, phase, error, msglvl;
  /* Auxiliary variables. */
  double ddum; /* Double dummy */
  MKL_INT idum; /* Integer dummy. */
  /* -------------------------------------------------------------------- */
  /* .. Setup Pardiso control parameters. */
  /* -------------------------------------------------------------------- */
  //mkl_set_num_threads(16);
  for (i = 0; i < 64; i++) 
  {
    iparm[i] = 0;
  }
  iparm[0] = 1; /* используются значения не по умолчанию, а приведенные ниже*/
  iparm[1] = 3; /* последовательная версия переупорядочивания, Fill-in reordering from METIS */    
  iparm[2] = 1;/* Numbers of processors, value of OMP_NUM_THREADS */
  iparm[3] = 0; /* итерационное уточнение не включается, No iterative-direct algorithm */
  iparm[4] = 1; /* вектор перестановки не записывается */
  iparm[5] = 0; /* решение записывается в вектор  resX */
  iparm[6] = 0; /* кол-во шагов итерационного уточнения, Not in use */
  iparm[7] = 2; /* Max numbers of iterative refinement steps */
  iparm[8] = 0; /* Not in use */
  iparm[9] = 13; /* Perturb the pivot elements with 1E-13 */
  iparm[10] = 1; /* Use nonsymmetric permutation and scaling MPS */
  iparm[11] = 0; /* Not in use */
  iparm[12] = 0; /* Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
  iparm[13] = 0; /* Output: Number of perturbed pivots */
  iparm[14] = 0; /* Not in use */
  iparm[15] = 0; /* Not in use */
  iparm[16] = 0; /* Not in use */
  iparm[17] = -1; /* сюда запишется число не нулей в факторе, Output: Number of nonzeros in the factor LU */
  iparm[18] = -1; /* Output: Mflops for LU factorization */
  iparm[19] = 0; /* Output: Numbers of CG Iterations */
  maxfct = 1; /* количество матриц, Maximum number of numerical factorizations. */
  mnum = 1; /* номер матрицы, Which factorization to use. */
  msglvl = 1; /*печатаем отчет, Print statistical information in file */
  error = 0; /*ошибок нет, Initialize error flag */
  /* -------------------------------------------------------------------- */
  /* .. Initialize the internal solver memory pointer. This is only */
  /* necessary for the FIRST call of the PARDISO solver. */
  /* -------------------------------------------------------------------- */
  for (i = 0; i < 64; i++) 
  {
    pt[i] = 0;
  }
  //приводим массивы к фортрановскому виду
  for(i = 0; i <= n; i++)
  {
    row[i]++;
  }
  for(i = 0; i < nz; i++)
  {
    column[i]++;
  }
  /* -------------------------------------------------------------------- */
  /* .. Reordering and Symbolic Factorization. This step also allocates */
  /* all memory that is necessary for the factorization. */
  /* -------------------------------------------------------------------- */
  phase = 13;// анализ структуры разреженности и символическая факторизация
  PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
    &n, val, row, column, iPerm, &nrhs,
    iparm, &msglvl,  b, resX, &error);
  if (error != 0) 
  {
    printf("\nERROR : %d", error);
    exit(1);
  }
  /* -------------------------------------------------------------------- */
  /* .. Termination and release of memory. */
  /* -------------------------------------------------------------------- */
  phase = -1; /* освобождение памяти, Release internal memory. */
  PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
    &n, &ddum, row, column, &idum, &nrhs,
    iparm, &msglvl, &ddum, &ddum, &error);

  //возвращение к нашим индексам
  for(i = 0; i <= n; i++)
  {
    row[i]--;
  }
  for(i = 0; i < row[n]; i++)
  {
    column[i]--;
  }
}


//определение детерминанта с помощью mkl
double MKL_Determinant(int n, int m, int * column, int * row, FLOAT_TYPE *val,FLOAT_TYPE *b, FLOAT_TYPE* resX)
{
  MKL_INT i;
  /* Allocate storage for the solver handle and the right-hand side. */

  _MKL_DSS_HANDLE_t handle;
  _INTEGER_t error;
  _CHARACTER_t statIn[] = "determinant";
  _DOUBLE_PRECISION_t statOut[5];
  MKL_INT opt = MKL_DSS_DEFAULTS;
  MKL_INT sym = MKL_DSS_SYMMETRIC;
  MKL_INT type = MKL_DSS_POSITIVE_DEFINITE;
  double result;
  int nNonZeros = row[n];
  for(i = 0; i <= n; i++)
  {
    row[i]++;
  }
  for(i = 0; i < nNonZeros; i++)
  {
    column[i]++;
  }
  /* --------------------- */
  /* Initialize the solver */
  /* --------------------- */
  error = dss_create(handle, opt );
  if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);
  /* ------------------------------------------- */
  /* Define the non-zero structure of the matrix */
  /* ------------------------------------------- */
  error = dss_define_structure(
    handle, sym, row, n, n,
    column, nNonZeros );
  if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);
  /* ------------------ */
  /* Reorder the matrix */
  /* ------------------ */
  error = dss_reorder( handle, opt, 0);
  if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);
  /* ------------------ */
  /* Factor the matrix */
  /* ------------------ */
  error = dss_factor_real( handle, type, val );
  if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);
  
  error = dss_solve_real( handle, opt, b, m, resX );
	if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);

  error = dss_statistics(handle, opt, statIn, statOut);
  if ( error != MKL_DSS_SUCCESS )printf("Solver returned error code %d\n", error);
  /*-------------------------*/
  /* print determinant */
  /*-------------------------*/
  printf(" determinant power is %g \n", statOut[0]);
  printf(" determinant base is %g \n", statOut[1]);
  printf(" Determinant is %g \n", (pow(10.0,statOut[0]))*statOut[1]);
  result = (pow(10.0,statOut[0]))*statOut[1];
  /* -------------------------- */
  /* Deallocate solver storage */
  /* -------------------------- */
  error = dss_delete( handle, opt );
  if ( error != MKL_DSS_SUCCESS ) printf("Solver returned error code %d\n", error);
  /* ---------------------- */
  /* Print solution vector */
  /* ---------------------- */

  for(i = 0; i <= n; i++)
  {
    row[i]--;
  }
  for(i = 0; i < row[n]; i++)
  {
    column[i]--;
  }
  return result;
}