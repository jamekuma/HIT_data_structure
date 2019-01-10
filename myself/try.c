#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct LINK_NODE
{
    int data;
    struct LINK_NODE *next;
} LINK_NODE, *LINK, *pos;

int search_DaoShu(LINK L, int k)
{
    pos p, q;
    p = q = L->next;
    if (k <= 0)
    {
        return 0;
    }
    while (k != 0 & p != NULL)
    {
        p = p->next;
        k--;
    }
    if (k != 0)
        return 0;
    while (p)
    {
        p = p->next;
        q = q->next;
    }
    printf("%d\n", q->data);
    return 1;
}

int main()
{
    LINK_NODE L;
    L.next = NULL;
    for (int i = 0; i < 7; i++)
    {
        pos s = (pos)malloc(sizeof(LINK_NODE));
        s->data = i;
        s->next = L.next;
        L.next = s;
    }
    search_DaoShu(&L, 8);
    return 0;
}
