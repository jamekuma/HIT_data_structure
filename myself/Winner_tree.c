#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 1000
#define INF 0x7fffffff
#define MIN 0x80000000
typedef int LIST[MAX];

typedef struct
{
    int key;
    int index;
} Tree_Node;

typedef Tree_Node Tree[MAX];
int initWinnerTree(LIST A, int n, Tree T)
{
    if (n % 2) //n % 2 == 1
    {
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
        for (int i = n; i <= 2 * n - 1; i++)
        {
            T[i].key = A[i - n];
            T[i].index = i;
        }
        return 2 * n - 1;
    }
}

int buildWinnerTree(Tree T, int len)
/*return first min index*/
{
    for (int i = len; i > 1; i -= 2)
    {
        T[i / 2] = T[i].key < T[i - 1].key ? T[i] : T[i - 1];
    }
    return T[1].index;
}

int reBuildWinnerTree(Tree T, int LastminIndex)
/*return index*/
{
    for (int i = LastminIndex; i > 1; i /= 2)
    {
        if (i % 2) // i % 2 == 1
        {
            T[i / 2] = T[i].key < T[i - 1].key ? T[i] : T[i - 1];
        }
        else
        {
            T[i / 2] = T[i].key < T[i + 1].key ? T[i] : T[i + 1];
        }
    }
    return T[1].index;
}

void WinnerSort(LIST A, int n, LIST B)
{
    Tree T;
    int len = initWinnerTree(A, n, T);
    int index = buildWinnerTree(T, len);
    for (int i = 0; i < n; i++)
    {
        B[i] = T[index].key;
        T[index].key = INF;
        index = reBuildWinnerTree(T, index);
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
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
        scanf("%d", &A[i]);

    printList(A, n);
    WinnerSort(A, n, B);
    printList(B, n);
    return 0;
}
