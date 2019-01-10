#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX 1000

typedef struct
{
    int top;
    double data[MAX];
} data_stack;   //数字栈

typedef struct
{
    int top;
    char ch[MAX];
} ch_stack;    //符号栈

typedef data_stack *D_STACK;   //标识数据栈的位置指针
typedef ch_stack *C_STACK;     //标识符号栈的位置指针

D_STACK buildDataStack(void)   //建立空的数据栈
{
    D_STACK st_d;
    st_d = (D_STACK)malloc(sizeof(data_stack));
    st_d->top = -1;
    return st_d;
}

C_STACK buildCharStack(void)  //建立空的符号栈
{
    C_STACK st_c;
    st_c = (C_STACK)malloc(sizeof(ch_stack));
    st_c->top = -1;
    return st_c;
}

void printCharStack(char *s, C_STACK st_c)   //打印当前符号栈，到目录s下
{
    static FILE *fp = NULL;
    if (fp == NULL)
    {
        if ((fp = fopen(s, "w+")) == NULL)
        {
            printf("open failed!\n");
            exit(1);
        }
    }

    if (st_c->top == -1)
    {
        fprintf(fp, "|_____|\n");
    }
    else
    {
        int i;
        for (i = st_c->top; i >= 0; i--)
        {
            fprintf(fp, "|__%c__|\n", st_c->ch[i]);
        }
    }
    fprintf(fp, "\n");
}
void printDataStack(char *s, D_STACK st_d)   //打印当前数据栈，到s目录下
{
    static FILE *fp = NULL;
    if (fp == NULL)
    {
        if ((fp = fopen(s, "w+")) == NULL)
        {
            printf("open failed!\n");
            exit(1);
        }
    }

    if (st_d->top == -1)    //栈空
    {
        fprintf(fp, "|_____|\n");
    }
    else
    {
        int i;
        for (i = st_d->top; i >= 0; i--)
        {
            fprintf(fp, "|__%.2lf__|\n", st_d->data[i]);
        }
    }
    fprintf(fp, "\n");
}


void pushDataStack(D_STACK st_d, double x)  //元素入数据栈
{
    st_d->data[++st_d->top] = x;
}

void pushCharStack(C_STACK st_c, char ch)  //元素入符号栈
{
    st_c->ch[++st_c->top] = ch;
}

double popDataStack(D_STACK st_d)    //数据元素出栈
{
    double d;
    d = st_d->data[st_d->top--];
    return d;
}

char popCharStack(C_STACK st_c)  //符号元素出栈
{
    char ch;
    ch = st_c->ch[st_c->top--];
    return ch;
}

int signToIndex(char ch)   //把运算符转换为数组下标
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
    case ')':
        index = 5;
        break;
    case '#':
        index = 6;
        break;
    case '^': 
        index = 7;
        break;
    default:
        printf("sign is error!!\n");
        exit(1);
    }
    return index;
}

bool priority(char ch1, char ch2)   //判断两运算符的优先级，ch1 > ch2 则返回true
{
    int index1, index2;
    index1 = signToIndex(ch1);
    index2 = signToIndex(ch2);
    bool table[8][8] =         //优先级表
        {//  *  /  +  -  (  )  #  ^
            {0, 0, 1, 1, 1, 1, 1, 0},  //  *
            {0, 0, 1, 1, 1, 1, 1, 0},  //  /
            {0, 0, 0, 0, 1, 1, 1, 0},  //  +
            {0, 0, 0, 0, 1, 1, 1, 0},  //  -
            {1, 1, 1, 1, 1, 0, 1, 1},  //  (
            {0, 0, 0, 0, 1, 0, 1, 0},  //  )
            {0, 0, 0, 0, 0, 0, 1, 0},  //  #
            {1, 1, 1, 1, 1, 1, 1, 0}}; //  ^
    return table[index1][index2];
}

void readAndprint(char *s_r, char *s_w, C_STACK st_c) //读取一个文件的中缀表达式，向另一个文件中写入后缀表达式
{
    FILE *fp_r, *fp_w;
    char temp[MAX];
    char *cp = NULL;
    char ch;
    double temp_data;
    if ((fp_r = fopen(s_r, "r+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    if ((fp_w = fopen(s_w, "w+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    fgets(temp, MAX, fp_r);   //将一行字符读进字符数组中
    cp = temp;
    while (*cp != '\0')    //依次处理每个字符，读到结束符为止
    {
        if (*cp <= '9' && *cp >= '0')   //若读到数字字符
        {
            sscanf(cp, "%lf", &temp_data);       //暂存数字
            // pushDataStack(st_d, temp_data);
            while (*cp <= '9' && *cp >= '0' || *cp == '.')    //将指针指到非数字为止
                cp++;
            fprintf(fp_w, "%.2lf ", temp_data);  //输出数
        }
        else      //读到其他字符      
        {
            switch (*cp)
            {
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case '#':
            case '^':
                while (st_c->top != -1 && !priority(*cp, st_c->ch[st_c->top]))  //若读到的运算符优先级低于栈顶运算符的优先级
                {
                    ch = popCharStack(st_c);                
                    if (ch != '(' && ch != ')')
                        fprintf(fp_w, "%c ", ch);    /*栈顶元素出栈，并打印( '('和')'不打印 )
                                                      直到读到的运算符优先级高于于栈顶运算符的优先级为止*/
                }
                pushCharStack(st_c, *cp);        //运算符入栈
                printCharStack(".\\progress_char.txt", st_c);  //打印符号栈的状态
            case ' ':
            case '.':
            case '\n':
                break;
            default:
                printf("sign is error!\n");
                exit(1);
            }
            cp++;
        }
    }
    fclose(fp_r);
    fclose(fp_w); //关闭文件
}

double readAndFigure(char *s)   //读取一个文件中的后缀表达式，计算出值，返回结果
{
    FILE *fp;
    D_STACK st_d = buildDataStack();
    char temp[MAX];
    char *cp;
    double temp_data;
    if ((fp = fopen(s, "r+")) == NULL)
    {
        printf("open failed!\n");
        exit(1);
    }
    fgets(temp, MAX, fp);
    cp = temp;
    while (*cp != '\0')
    {
        if (*cp <= '9' && *cp >= '0')
        {
            sscanf(cp, "%lf", &temp_data);
            pushDataStack(st_d, temp_data);
            printDataStack(".\\progress_data.txt", st_d);
            while (*cp <= '9' && *cp >= '0' || *cp == '.')
                cp++;
        }
        else
        {
            switch (*cp)
            {
            case '+':  //栈顶两个元素相加
                st_d->data[st_d->top - 1] = st_d->data[st_d->top - 1] + st_d->data[st_d->top];
                st_d->top--;
                printDataStack(".\\progress_data.txt", st_d);
                break;
            case '-':  //栈顶两个元素相减
                st_d->data[st_d->top - 1] = st_d->data[st_d->top - 1] - st_d->data[st_d->top];
                st_d->top--;
                printDataStack(".\\progress_data.txt", st_d);
                break;
            case '*':  //栈顶两个元素相乘
                st_d->data[st_d->top - 1] = st_d->data[st_d->top - 1] * st_d->data[st_d->top];
                st_d->top--;
                printDataStack(".\\progress_data.txt", st_d);
                break;
            case '/':  //栈顶两个元素相除
                st_d->data[st_d->top - 1] = st_d->data[st_d->top - 1] / st_d->data[st_d->top];
                st_d->top--;
                printDataStack(".\\progress_data.txt", st_d);
                break;
            case '^':  //栈顶两个元素乘方
                st_d->data[st_d->top - 1] = pow(st_d->data[st_d->top - 1], st_d->data[st_d->top]);
                st_d->top--;
                printDataStack(".\\progress_data.txt", st_d);
                break;
            case ' ':
            case '.':
            case '\n':
                break;
            default:
                printf("sign is error!!\n");
                exit(1);
            }
            cp++;
        }
    }
    return (popDataStack(st_d)); //返回栈中剩下的唯一的元素
}

int main()
{
    D_STACK st_d = buildDataStack();
    C_STACK st_c = buildCharStack();
    readAndprint(".\\input.txt",
                 ".\\postfix.txt", st_c);
    printf("The value is %.2lf\n", readAndFigure(".\\postfix.txt"));
    return 0;
}