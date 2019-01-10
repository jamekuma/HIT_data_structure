#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define MAX 1000

typedef char datatype;

struct node
{
    struct node *lchild;
    struct node *rchild;
    datatype data;
};

typedef struct node *NODE;
typedef struct node *BTREE;


int counter = 0;       //计数器，记录树的节点个数
BTREE buildRecur(void) //以先序顺序递归建树 
{
    BTREE t;
    datatype ch;
    scanf("%c", &ch);
    if (ch == '#')   //若输入'#'证明遇到空指针
    {
        t = NULL;
    }
    else             //否则建立一个新的节点
    {
        t = (BTREE)malloc(sizeof(struct node));  //开辟新节点
        t->data = ch;                         //填入数据
        counter++;                           //计数器加一
        t->lchild = buildRecur();  
        t->rchild = buildRecur();            //建立当前的左节点和右节点
    }
    return t;                               //返回建立的节点
}

BTREE buildNoRecur(void)  //非递归建树
{
    BTREE t;         //储存根节点
    NODE s[MAX], p;  
    int i, j;        
    datatype ch;
    scanf("%d %c", &i, &ch);
    getchar();
    while (i != 0 && ch != 0)
    {
        p = (BTREE)malloc(sizeof(struct node));
        p->data = ch;
        counter++;
        s[i] = p;    
        p->lchild = NULL;  
        p->rchild = NULL;
        if (i == 1)   //如果是父节点，
            t = p;
        else
        {
            j = i / 2;   // 不是父节点，则父节点为i / 2
            if (i % 2 == 0)      
                s[j]->lchild = p;   //是左孩子
            else                    
                s[j]->rchild = p;   //是右孩子
        }
        scanf("%d %c", &i, &ch);
        getchar();
    }
    return t;
}

void preRecur(BTREE root)    //先序遍历（递归）
{
    if (root != NULL)        
    {
        if (root->data != '$')
            printf("%c ", root->data);
        preRecur(root->lchild);
        preRecur(root->rchild);
    }
}
void preNoRecur(BTREE root)  //先序遍历（非递归）
{
    NODE stack[MAX];
    int top = -1; 
    while (top != -1 || root != NULL)   
    {
        while (root != NULL)
        {
            if (root->data != '$')
                printf("%c ", root->data);
            if (root->rchild != NULL)
            {
                stack[++top] = root;
            }
            root = root->lchild;
        }
        if (top != -1)
        {
            root = stack[top--];
            root = root->rchild;
        }
    }
}

void inRecur(BTREE root)       //中序遍历（递归）
{
    if (root != NULL)
    {
        inRecur(root->lchild);
        if (root->data != '$')
            printf("%c ", root->data);
        inRecur(root->rchild);
    }
}

void inNoRecur(BTREE root)    //中序遍历（非递归）
{
    NODE stack[MAX];
    int top = -1;
    while (root != NULL || top != -1)
    {
        while (root != NULL)
        {
            // if (root->rchild != NULL)
            stack[++top] = root;
            root = root->lchild;
        }

        if (top != -1)
        {
            root = stack[top--];
            if (root->data != '$')
                printf("%c ", root->data);
            root = root->rchild;
        }
    }
}

void postRecur(BTREE root)    //后序遍历（递归）
{
    if (root != NULL)
    {
        postRecur(root->lchild);
        postRecur(root->rchild);
        if (root->data != '$')
            printf("%c ", root->data);
    }
}

void postNoRecur(BTREE root)  //后序遍历（非递归）
{
    typedef struct
    {
        int flag;
        NODE node;
    } STACK;

    STACK stack[MAX];
    NODE temp;
    int top = -1;
    while (root != NULL || top != -1)   //循环直到root为空且栈s为空

    {
        while (root != NULL)    //当root非空时循环
        {
            stack[++top].node = root;  
            stack[top].flag = 1;    //将root连同标志flag=1入栈
            root = root->lchild;   //继续遍历root的左子树
        }
        while (top != -1 && stack[top].flag == 2)     //当栈顶标志为2循环
        {
            temp = stack[top--].node;
            if (temp->data != '$')
                printf("%c ", temp->data);     //出栈并输出
        }
        if (top != -1)        //若栈不为空
        {
            root = stack[top].node;  //读取栈顶
            stack[top].flag = 2;    //栈顶标志置2
            root = root->rchild;    //遍历栈的右孩子
        }
    }
}
void level(BTREE root)    //层序遍历
{
    NODE Q[MAX];        
    int front = 0, rear = 0;  //初始化队列
    if (root == NULL)        //如果树空，直接返回
        return;
    Q[rear++] = root;       //根节点入队
    while (rear != front)   //循环直到队列为空
    {
        root = Q[front++];           //出队
        if (root->data != '$')       
            printf("%c ", root->data);  //访问
        if (root->lchild != NULL)
            Q[rear++] = root->lchild;   //左孩子入队
        if (root->rchild != NULL)
            Q[rear++] = root->rchild;   //右孩子入队
    }
}


void signNode(BTREE root, bool num[], int i)  //对二叉树root，递归地构造一个布尔数组
{
    if (root != NULL && root->data != '$')
    {
        num[i] = 1;
        signNode(root->lchild, num, i * 2);
        signNode(root->rchild, num, i * 2 + 1);
    }
}

bool isComplete(BTREE root)   //判断是否为完全二叉树
{
    bool num[MAX];
    int i;
    memset(num, 0, sizeof(num));  //数组初始化为false
    signNode(root, num, 1);   
    i = 1;           
    while (num[i] == 1)
        i++; 
    if (i - 1 == counter)
        return true;    //如果数组的第1 ~ counter项都为true，则为完全二叉树
    else
        return false;
}

void setFloor(NODE root, int i, int numOfFloor[])  //递归遍历每个节点，构造记录每层节点数的数组
{
    if (root != NULL)
    {
        numOfFloor[i]++;
        setFloor(root->lchild, i + 1, numOfFloor);
        setFloor(root->rchild, i + 1, numOfFloor);
    }
}


int wide(BTREE root) //求宽度
{
    int numOfFloor[MAX];  
    int i, max;
    memset(numOfFloor, 0, sizeof(numOfFloor));
    setFloor(root, 1, numOfFloor);    
    i = 1;
    max = 0;
    while (numOfFloor[i] != 0)
    {
        max = (numOfFloor[i] > max) ? numOfFloor[i] : max;
        i++;
    }            //打擂算法找出最大的宽度
    return max;
}



int c = 0;  //记录应该输出字符的个数
void buildArray(BTREE root, datatype num[], int i)  //构造顺序存储二叉树的数组，为输出做准备
{
    if (root != NULL && root->data != '$')
    {
        num[i] = root->data;
        c++;
        buildArray(root->lchild, num, i * 2);
        buildArray(root->rchild, num, i * 2 + 1); 
    }
    else
    {
        num[i] = '#';            //空节点用#填充
        c++;
    }
}

void print(BTREE root)     //层序输出
{
    datatype num[MAX];
    memset(num, 0, sizeof(num));
    buildArray(root, num, 1);
    int i;
    for (i = 1; i < c + 1; i++)
    {
        printf("%c ", num[i]);
        if((i + 1 & i )== 0)   
            printf("\n");     //当i + 1为2的整数次方时，换行
    }
}

int main()
{
    BTREE t;

    /*build*/
    // t = buildNoRecur();
    t = buildRecur();

    printf("\nthe tree is \n");

    print(t);

    printf("\n\n");
 /*traverse*/

    printf("preOrder:\n");
    preRecur(t);
    // preNoRecur(t);
    printf("\n");

    printf("inOrder:\n");
    inRecur(t);
    // inNoRecur(t);
    printf("\n");

    printf("postOrder:\n");
    postRecur(t);
    // postNoRecur(t);
    printf("\n");

    printf("levelOrder:\n");
    level(t);
    printf("\n\n");

    /*is it complete?*/
    if (isComplete(t))
        printf("it\'s a complete binary tree \n");
    else
        printf("it isn\'t a complete binary tree \n");
    printf("\n");

    /*figue out the wide*/
    printf("the wide is %d\n\n", wide(t));
    // getchar();getchar();
    return 0;
}