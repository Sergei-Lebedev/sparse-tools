#ifndef TYPE
#define TYPE

#define EPSILON 1E-15

#define FLOAT_TYPE double

typedef struct
{
  int *vertexes; //списки номеров вершин по уровням
  int *vertexesStart; //индекс начала каждого уровня
  int *parents; //индекс родителя вершины в дереве
  int *childs; //индексы потомков вершины в дереве исключения
  int *childsStart; //индекс начала потомков вершины в массиве child
  int *nChilds; //число прямых потомков вершины
  int *nAllChilds; //число потомков вершины в поддереве с корнем в ней
  int nLevels; //число уровней
} EliminationTree; 

typedef struct
{
    int *vertexes; //списки вершин по уровням, начиная с листьев
    int *vertexesStart; //индекс начала каждого уровня
    int nLevels; //число уровней
} ParallelTree;

#endif