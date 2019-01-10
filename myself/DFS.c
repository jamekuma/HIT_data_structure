#include <stdio.h>
#define MAX 10000

typedef int datatype;

typedef struct NODE
{
    int adjvex;
    int cost;
    struct NODE *next;
}EdgeNode;

typedef struct 
{
    datatype data;
    EdgeNode *firstedge;
}VertexNode;

typedef struct
{
    VertexNode vexlist[MAX];
    int n;
    int e;
}AdjGragh;


// void DFS1()

int main()
{
    return 0;
}