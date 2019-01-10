#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 1000000

long exch_bubble = 0;
long exch_quick = 0;

typedef struct
{
    int key;
    int other;
} records;

typedef records LIST[MAX];

LIST B, Q;

void initRecords(LIST A, int num)
{
    srand(time(NULL));

    for (int i = 0; i < num; i++)
        A[i].key = rand();
}

void bubbleSort(LIST A, int n)
{
    bool isExchange;
    for (int i = 0; i < n - 1; i++)
    {
        isExchange = false;      
        for (int j = n - 1; j > i; j--)
        {
            if (A[j].key < A[j - 1].key)
            {
                isExchange = true;   //该轮无序

                records temp = A[j];
                A[j] = A[j - 1];
                A[j - 1] = temp;
            }
        }
        if (!isExchange)  //该轮有序，不用继续排序了
            return;             
    }
}

void printKey(LIST A, int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", A[i].key);
    printf("\n");
}

/*find the pivot node's value*/
int findPivot(int i, int j, LIST A)
{
    int fisrtKey = A[i].key;
    for (int k = i + 1; k <= j; k++)
    {
        if (A[k].key > fisrtKey)
            return A[k].key;
        if (A[k].key < fisrtKey)
            return fisrtKey;
    }
    return -1; //i ~ j 已经有序
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int i, int j, LIST A, int pivot)
{
    int r = i;
    int l = j;

    while (r < l)
    {
        swap(&(A[r].key), &(A[l].key));   //交换r l的key


        while (A[r].key < pivot)
            r++;
        while (A[l].key >= pivot)
            l--;
    }
    return l;             //返回基准点的下标
}

void quickSort(int i, int j, LIST A)
{
    int pivot;
    if ((pivot = findPivot(i, j, A)) != -1)
    {
        int k = partition(i, j, A, pivot);
        quickSort(i, k, A);
        quickSort(k + 1, j, A);   //分别对两边进行递归的排序
    }
}

int main()
{
    int num;  //数据组数
    srand(time(NULL));

    /*快速排序*/
    freopen(".\\data_quick.txt", "w", stdout);
    for (num = 0; num < 500; num++)
    {
        int len = rand() * 1000 % MAX; // 数据长度随机
        initRecords(Q, len);             // 随机生成一组排序数据
        int start = clock();
        quickSort(0, len - 1, Q);
        int quick_time = (clock() - start);

        printf("%d\t%d\n", len, quick_time); //生成时间数据
    }


    /*冒泡排序*/
    freopen(".\\data_bubble.txt", "w", stdout);
    for (num = 0; num < 100; num++)
    {
        int len = rand() % MAX;    // 数据长度随机
        initRecords(B, len);     // 随机生成一组排序数据

        int start = clock();
        bubbleSort(B, len);
        int bubble_time = (clock() - start);

        printf("%d\t%d\n", len, bubble_time); //生成数据
    }
    return 0;
}
