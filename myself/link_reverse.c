#include <stdio.h>
#include <stdlib.h>

typedef int datatype;

typedef struct NODE
{
    datatype data;
    struct NODE *next;
    struct NODE *last;
} node;

typedef struct NODE *LIST;
typedef struct NODE *pos;

void reverse(LIST head) //retrun new head
{
    pos p = head->next;
    pos q = p->next;
    pos r = q->next; //  p - q - r
    p->next = NULL;
    while (r != NULL)
    {
        q->next = p;
        p = q;
        q = r;
        r = r->next;
    }
    q->next = p;
    head->next = q;
}

void insert(datatype x, pos p)
{
    pos s = (pos)malloc(sizeof(node));
    s->data = x;
    s->next = p->next;
    p->next = s;
}

void delete (pos p)
{
    if (p->next != NULL)
    {
        pos s = p->next;
        p->next = s->next;
        free(s);
    }
}

void makeNull(LIST head)
{
    pos p = head;
    while (p->next != NULL)
    {
        delete (p);
        p = p->next;
    }
}

LIST build(void)
{
    LIST head = (LIST)malloc(sizeof(node));
    head->next = NULL;
    return head;
}
void print(LIST head)
{
    pos p;
    for (p = head->next; p != NULL; p = p->next)
        printf("%d ", p->data);
    printf("\n");
}

int main()
{
    LIST head1 = build();
    pos p = head1;
    datatype i;
    for (i = 0; i < 100; i++)
    {
        insert(i, p);
        p = p->next;
    }
    print(head1);
    reverse(head1);
    print(head1);
}
