#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 100000
#define INF 0x7fffffff
#define MIN 0x80000000
typedef int LIST[MAX];

typedef struct
{
    int key;
    int index;
} Tree_Node;

typedef Tree_Node Tree[MAX];

int initLoserTree(LIST A, int n, Tree T)  //init,return the number of all node
{
    if (n % 2)
    {
        for (int i = 1; i <= n; i++)
            T[i].key = MIN;
        for (int i = n + 1; i < 2 * n + 1; i++)
        {
            T[i].key = A[i - n - 1];
            T[i].index = i;
        }
        T[2 * n + 1].key = INF;
        T[2 * n + 1].index = 2 * n + 1;
        return 2 * n + 1;
    }
    else
    {
        for (int i = 1; i <= n - 1; i++)
            T[i].key = MIN;
        for (int i = n; i <= 2 * n - 1; i++)
        {
            T[i].key = A[i - n];
            T[i].index = i;
        }
        return 2 * n - 1;
    }
}

int adjust(Tree T, int index)
{
    Tree_Node winner = T[index];
    for (int i = index; i > 1; i /= 2)
    {
        if (winner.key > T[i / 2].key)   //if winner lost
        {
            Tree_Node temp = T[i / 2];
            T[i / 2] = winner; //save loser
            winner = temp;     //winner enter next turn
        }
    }
    T[0] = winner;
    return T[0].index;    
}

int buildLoserTree(Tree T, int len)
{
    for (int i = len; i >= len / 2 + 1; i--) //start from all leaf to root
        adjust(T, i);
    return T[0].index;
}

void LoserSort(LIST A, int n, LIST B)  
{
    Tree T;
    int len = initLoserTree(A, n, T);
    int index = buildLoserTree(T, len);
    for (int i = 0; i < n; i++)
    {
        B[i] = T[index].key;
        T[index].key = INF;        //index is has been sorted, set it INF 
        index = adjust(T, index);  //start from index
    }
}

void printList(LIST A, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", A[i]);
    printf("\n");
}

int main()
{
    freopen(".\\data.out", "w", stdout);
    freopen(".\\data.in", "r", stdin);

    LIST A, B;

    int n;          //number of each data
    int numOfData;  //sets of data

    scanf("%d", &numOfData);

    /*test*/
    for (int i = 0; i < numOfData; i++)  
    {
        scanf("%d", &n);

        for (int j = 0; j < n; j++)
            scanf("%d", &A[j]);

        printf("No.%d data:\n", i + 1);
        printf("num : %d\n", n);

        printf("Origin:\n");
        printList(A, n);

        LoserSort(A, n, B);   //sort

        printf("After sort:\n");
        printList(B, n);
        printf("\n");
    }
    return 0;
}
