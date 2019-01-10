#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX 1000
#define EMP '\0'

// typedef union
// {
//     int i_num;
//     double f_num;
//     char ch; 
// }datatype;

FILE *output_fp = NULL;
FILE *inout_fp = NULL;
FILE *progress_fp = NULL;
typedef struct
{
    int last;
    char data[MAX];
} sign_list;

typedef struct
{
    int top;
    char data[MAX];
} stack;

typedef sign_list *SIGN_LIST;
typedef stack *STACK;

STACK makeNullStack(void)
{
    STACK st = (STACK)malloc(sizeof(stack));
    st->top = -1;
    return st;
}

SIGN_LIST makeNullList(void)
{
    SIGN_LIST l = (SIGN_LIST)malloc(sizeof(sign_list));
    l->last = -1;
    return l;
}

void printList(SIGN_LIST l, char *s)
{
    int i;
    FILE *fp = NULL;
    if ((fp = fopen(s, "a")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    if (l->last == -1)
    {
        fprintf(fp, "sign_list is empty\n");
        // exit(1);
    }
    else
    {
        for (i = 0; i <= l->last; i++)
        {
            fprintf(fp, "%c ", l->data[i]);
        }
    }
    fprintf(fp, "\n\n");
}

void attachToSignList(SIGN_LIST l, char ch)
{
    if (l->last == MAX)
    {
        printf("sign_list is full\n");
        exit(1);
    }
    else
    {
        l->last++;
        l->data[l->last] = ch;
    }
}


char popStack(STACK st)
{
    char c;
    if (st->top == -1)
        return EMP;
    else
    {
        c = st->data[st->top];
        st->top--;
        return c;
    }
}

void pushStack(STACK st, char x)
{
    if (st->top == MAX - 1)
    {
        printf("stack is full!\n");
        exit(1);
    }
    else
    {
        st->top++;
        st->data[st->top] = x;
    }
}

void printStack(STACK st, char *s)
{
    FILE *fp = NULL;
    if ((fp = fopen(s, "a")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }

    if (st->top == -1)
    {
        fprintf(fp, "|_____|\n");
    }
    else
    {
        int i;
        for (i = st->top; i >= 0; i--)
        {
            fprintf(fp, "|__%c__|\n", st->data[i]);
        }
    }
    fprintf(fp, "\n");
}

int signToIndex(char ch)
{
    int index;
    switch (ch)
    {
    case '*':
        index = 0;
        break;
    case '/':
        index = 1;
        break;
    case '+':
        index = 2;
        break;
    case '-':
        index = 3;
        break;
    case '(':
        index = 4;
        break;
    // case ')':
    //     index = 5;
    //     break;
    default:
        printf("sign is error!\n");
        exit(1);
    }
    return index;
}

bool priority(char ch1, char ch2)
{
    int index1, index2;
    index1 = signToIndex(ch1);
    index2 = signToIndex(ch2);
    bool table[5][5] =
        {
            {0, 0, 1, 1, 1},
            {0, 0, 1, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0}};
    return table[index1][index2];
}

SIGN_LIST read(char *s)
{
    FILE *fp;
    char ch;
    SIGN_LIST l = makeNullList();
    if ((fp = fopen(s, "r")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch != ' ' && ch != '\n' && ch != '\r')
            attachToSignList(l, ch);
    }
    return l;
}

bool isOP(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
        return true;
    default:
        return false;
    }
}

SIGN_LIST buildPost(SIGN_LIST form, char *s)
{
    FILE *fp = NULL;
    if ((fp = fopen(s, "w+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    SIGN_LIST postForm = makeNullList();
    STACK st = makeNullStack();
    char *p, temp;
    p = form->data;
    while (p <= form->data + form->last)
    {
        if (isOP(*p))
        {
            if (*p == ')')
            {
                while ((temp = popStack(st)) != '(')
                {
                    attachToSignList(postForm, temp);
                    if (st->top == -1)
                    {
                        printf("bracket is not match!\n");
                        exit(1);
                    }
                    // printStack(st, s);
                    // printList(postForm, s);
                }
                p++;
            }
            else if (st->top == -1 || priority(*p, st->data[st->top]))
            {
                pushStack(st, *p);

                // printStack(st, s);
                // printList(postForm, s);
                p++;
            }
            else
            {
                temp = popStack(st);
                attachToSignList(postForm, temp);
                // printStack(st, s);
                // printList(postForm, s);
            }
        }
        else
        {
            attachToSignList(postForm, *p);
            p++;
            // printStack(st, s);
            // printList(postForm, s);
        }
        printStack(st, s);
        printList(postForm, s);
    }
    while (st->top > -1)
    {
        temp = popStack(st);
        attachToSignList(postForm, temp);
        printStack(st, s);
        printList(postForm, s);
    }
    return postForm;
}


int main()
{
    SIGN_LIST form = read("F:\\data_structure\\task\\task1\\origin\\output.txt");
    SIGN_LIST postForm = buildPost(form, "F:\\data_structure\\task\\task1\\origin\\output.txt");

    printList(postForm, "F:\\data_structure\\task\\task1\\origin\\output.txt");
    return 0;
}