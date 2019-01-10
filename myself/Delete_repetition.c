#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct
{
    int length;
    int data[1000];
}LIST_NODE;

typedef LIST_NODE *LIST;

void delete_repetition(LIST L)
{
    int k = 0, temp = L->data[0];
    for (int i = 1; i < L->length; i++)
    {
        if (L->data[i] == temp)
            k++;
        else 
        {
            temp = L->data[i];
            L->data[i - k] = L->data[i];   
        }
    }
    L->length = L->length - k;
}

int main()
{
    LIST_NODE L;
    // L.data[] = {1, 2, 2, 2, 3, 3, 3, 4, 4, 5};
    L.data[0] = 1;
    L.data[1] = 2;
    L.data[2] = 2;
    L.data[3] = 2;
    L.data[4] = 2;
    L.data[5] = 2;
    L.data[6] = 2;
    L.length = 7;
    delete_repetition(&L);
    for (int i = 0; i < L.length; i++)
        printf("%d ", L.data[i]);
    printf("\n");
    return 0;
}
