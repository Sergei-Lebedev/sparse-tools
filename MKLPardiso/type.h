#ifndef TYPE
#define TYPE

#define EPSILON 1E-15

#define FLOAT_TYPE double

typedef struct
{
  int *vertexes; //������ ������� ������ �� �������
  int *vertexesStart; //������ ������ ������� ������
  int *parents; //������ �������� ������� � ������
  int *childs; //������� �������� ������� � ������ ����������
  int *childsStart; //������ ������ �������� ������� � ������� child
  int *nChilds; //����� ������ �������� �������
  int *nAllChilds; //����� �������� ������� � ��������� � ������ � ���
  int nLevels; //����� �������
} EliminationTree; 

typedef struct
{
    int *vertexes; //������ ������ �� �������, ������� � �������
    int *vertexesStart; //������ ������ ������� ������
    int nLevels; //����� �������
} ParallelTree;

#endif