#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
#include <string.h>  
#include <stdbool.h>  
#define MAX 10000  
  
typedef struct  
{  
    int parent[MAX];  
    int n;  
} Tree; //����㣩������  
  
Tree t1, t2, t3, t4, t5, t6;  
  
typedef int datatype;  
  
typedef struct  
{  
    datatype vertex[MAX];  
    bool edge[MAX][MAX];  
    int n, e;  
} UDgraph_ED_NODE; //�ڽӾ����ʾ��ͼ  
  
typedef struct EDGENODE  
{  
    int adjvex;  
    struct EDGENODE *next;  
} EdgeNode; //�ڽӱ�ı߱�ڵ�  
  
typedef struct  
{  
    datatype vertex;  
    EdgeNode *firstedge;  
} VertexNode; //�ڽӱ�Ķ����ڵ�  
  
typedef struct  
{  
    VertexNode vexlist[MAX];  
    int n, e;  
} UDgraph_ADJ_NODE; //�ڽӱ��ʾ��ͼ  
  
typedef UDgraph_ADJ_NODE *UDgraph_ADJ;  
typedef UDgraph_ED_NODE *UDgraph_ED;  
  
UDgraph_ED makeNull_ED(void) //��ʼ��һ���յ��ڽӾ���ͼ  
{  
    UDgraph_ED G = (UDgraph_ED)malloc(sizeof(UDgraph_ED_NODE));  
    for (int i = 0; i < MAX; i++)  
        for (int j = 0; j < MAX; j++)  
            G->edge[i][j] = 0;  
    G->e = 0;  
    G->n = 0;  
    return G;  
}  
  
UDgraph_ADJ makeNull_ADJ(void) //��ʼ��һ���յ��ڽӱ�ͼ  
{  
    UDgraph_ADJ G = (UDgraph_ADJ)malloc(sizeof(UDgraph_ADJ_NODE));  
    G->n = 0;  
    G->e = 0;  
    for (int i = 0; i < MAX; i++)  
        G->vexlist[i].firstedge = NULL;  
    return G;  
}  
  
bool isEdgeRight(UDgraph_ED G) //�ж�����ı��Ƿ���ȷ  
{  
    int e_count = 0;  
  
    for (int i = 0; i < G->n; i++)  
    {  
        for (int j = 0; j < G->n; j++)  
        {  
            if (G->edge[i][j])  
                e_count++;  
            if (G->edge[i][j] != G->edge[j][i])  
                return false;  
            if (i == j && G->edge[i][j] == 1)  
                return false;  
        }  
    }  
  
    if (e_count / 2 == G->e)  
        return true;  
    else  
        return false;  
}  
  
// void readNumbers(int n, int)  
  
UDgraph_ED build_ED(char *path) //�����ڽӾ���ͼ  
{  
    UDgraph_ED G = makeNull_ED();  
  
    FILE *fp;  
    if ((fp = fopen(path, "r+")) == NULL)  
    {  
        printf("open failed!\n");  
        exit(1);  
    }  
  
    char s[MAX];  
    int line = 0;  
  
    int n_input, e_input;  
  
    char *f_ch;  
    int node_num = 0;  
    int counter;  
    int row = 0, column;  
  
    while (fgets(s, MAX, fp) != NULL)  
    {  
        line++; //add line  
        switch (line)  
        {  
        case 1: //line 1  
            sscanf(s, "%d", &n_input);  
            G->n = n_input;  
            break;  
        case 2: //line 2  
            f_ch = s;  
            counter = 0;  
            while (*f_ch)  
            {  
                if (*f_ch >= '0' && *f_ch <= '9') //if get number  
                {  
                    sscanf(f_ch, "%d", &(G->vertex[counter]));  
                    counter++;  
                    while (*f_ch >= '0' && *f_ch <= '9') //jump out of number  
                        f_ch++;  
                    f_ch--;  
                }  
                f_ch++;  
            }  
            if (counter != n_input) //if num is mismatching,print warning  
                printf("warning: there is node which is not intialized!\n");  
            break;  
  
        case 3: //line 3  
            sscanf(s, "%d", &e_input);  
            G->e = e_input;  
            break;  
  
        default: //input edge  
            f_ch = s;  
  
            column = 0;  
            while (*f_ch)  
            {  
                if (*f_ch == '1')  
                    G->edge[row][column++] = 1;  
                else if (*f_ch == '0')  
                    G->edge[row][column++] = 0;  
                f_ch++;  
            }  
            if (column != n_input)  
            {  
                printf("the edge input error!\n");  
                exit(2);  
            }  
            row++;  
        }  
    }  
    if (!isEdgeRight(G)) //�жϱߵĺϷ���  
    {  
        printf("the the edge input error!\n");  
        exit(3);  
    }  
    fclose(fp);  
    return G;  
}  
  
UDgraph_ADJ build_ADJ(char *path) //�����ڽӱ�ͼ  
{  
    UDgraph_ADJ G = makeNull_ADJ();  
    FILE *fp;  
    if ((fp = fopen(path, "r+")) == NULL)  
    {  
        printf("open failed!\n");  
        exit(1);  
    }  
    char s[MAX];  
    char *f_ch;  
    int line = 0, n_input, e_input;  
    int e_counter = 0;  
    int n1, n2;  
    while (fgets(s, MAX, fp) != NULL)  
    {  
        line++;  
        switch (line)  
        {  
        case 1:  
            sscanf(s, "%d", &n_input);  
            break;  
  
        case 2:  
            f_ch = s;  
            int counter = 0;  
            while (*f_ch)  
            {  
                if (*f_ch >= '0' && *f_ch <= '9')  
                {  
                    sscanf(f_ch, "%d", &(G->vexlist[counter]));  
                    counter++;  
                    while (*f_ch >= '0' && *f_ch <= '9')  
                        f_ch++;  
                    f_ch--;  
                }  
                f_ch++;  
            }  
            if (counter != n_input)  
                printf("warning: There is vexlist which is not initialized!\n");  
            break;  
        case 3:  
            sscanf(s, "%d", &e_input);  
            break;  
        default:  
            if (sscanf(s, "%d %d", &n1, &n2) != 2 || n1 < 0 || n1 >= n_input || n2 < 0 || n2 >= n_input)  
            {  
                printf("input error!\n");  
                exit(2);  
            }  
  
            int flag = 0;  
            EdgeNode *p = G->vexlist[n1].firstedge;  
            while (p)  
            {  
                if (p->adjvex == n2)  
                    flag = 1;  
                p = p->next;  
            }  
            if (!flag && n1 != n2)  
            {  
                p = (EdgeNode *)malloc(sizeof(EdgeNode));  
                p->adjvex = n2;  
                p->next = G->vexlist[n1].firstedge;  
                G->vexlist[n1].firstedge = p;  
  
                p = (EdgeNode *)malloc(sizeof(EdgeNode));  
                p->adjvex = n1;  
                p->next = G->vexlist[n2].firstedge;  
                G->vexlist[n2].firstedge = p;  
  
                e_counter++;  
            }  
        }  
    }  
    if (e_counter != e_input)  
    {  
        printf("the number of edge is error!\n");  
        exit(2);  
    }  
  
    G->n = n_input;  
    G->e = e_input;  
    return G;  
}  
  
void printEdge(UDgraph_ED G) //��ӡͼ���ڽӱ��ʾ  
{  
    for (int i = 0; i < G->n; i++)  
    {  
        for (int j = 0; j < G->n; j++)  
            printf("%d ", G->edge[i][j]);  
        printf("\n");  
    }  
}  
  
void ED2ADJ(UDgraph_ADJ G_adj, UDgraph_ED G_ed) //�ڽӾ�����ʽתΪ�ڽӱ���ʽ  
{  
    int i, j;  
    EdgeNode *p_edgeNode;  
  
    G_adj->n = G_ed->n;  
    G_adj->e = G_ed->e;  
    for (i = 0; i < G_ed->n; i++)  
        G_adj->vexlist[i].vertex = G_ed->vertex[i];  
    for (i = 0; i < G_ed->n; i++)  
    {  
        for (j = 0; j < G_ed->n; j++)  
        {  
            if (G_ed->edge[i][j])  
            {  
                p_edgeNode = (EdgeNode *)malloc(sizeof(EdgeNode));  
                p_edgeNode->adjvex = j;  
                p_edgeNode->next = G_adj->vexlist[i].firstedge;  
                G_adj->vexlist[i].firstedge = p_edgeNode;  
            }  
        }  
    }  
}  
  
void ADJ2ED(UDgraph_ED G_ed, UDgraph_ADJ G_adj) //�ڽӾ�����ʽתΪ�ڽӾ�����ʽ  
{  
    int i, j;  
    EdgeNode *p_edgeNode;  
    G_ed->n = G_adj->n;  
    G_ed->e = G_adj->e;  
  
    for (i = 0; i < G_adj->n; i++)  
        G_ed->vertex[i] = G_adj->vexlist[i].vertex;  
    for (i = 0; i < G_adj->n; i++)  
    {  
        p_edgeNode = G_adj->vexlist[i].firstedge;  
        while (p_edgeNode)  
        {  
            G_ed->edge[i][p_edgeNode->adjvex] = 1;  
            p_edgeNode = p_edgeNode->next;  
        }  
    }  
}  
  
void DFS_ED_Recur(UDgraph_ED G, int start, bool visited[], int dfn[], int *counter) //��ĳ���ڵ㿪ʼ����ݹ飨�ڽӾ���  
{  
    visited[start] = true;  
    printf("%d ", G->vertex[start]); //����ڵ�  
    dfn[start] = *counter;           //������������  
    (*counter)++;                    //��������һ  
    for (int i = 0; i < G->n; i++)  
    {  
        if (G->edge[start][i] == 1 && visited[i] == false) //������һ���ڽӵģ�δ�����ʵĽڵ�  
        {  
            DFS_ED_Recur(G, i, visited, dfn, counter);  
            t1.parent[i] = start; //��������������  
        }  
    }  
}  
  
int *DFSTraverse_ED_Recur(UDgraph_ED G) //����ݹ飨�ڽӾ���  
{  
    int *dfn = (int *)calloc(G->n, sizeof(int));  
    int counter = 0;  
    bool visited[MAX];  
  
    for (int i = 0; i < MAX; i++) //��ʼ������������  
        t1.parent[i] = -1;  
  
    for (int i = 0; i < G->n; i++)  
        visited[i] = false; //��ʼ����������  
  
    for (int i = 0; i < G->n; i++) //��ÿ��δ�����ʵĽڵ㿪ʼ  
        if (!visited[i])  
            DFS_ED_Recur(G, i, visited, dfn, &counter);  
    return dfn;  
}  
  
void DFS_ADJ_Recur(UDgraph_ADJ G, int start, bool visited[], int dfn[], int *counter)  
//��ĳ���ڵ㿪ʼ����ݹ飬���ڽӱ� ������ͬ��  
{  
    visited[start] = true;  
    printf("%d ", G->vexlist[start].vertex);                                //����ڵ�  
    dfn[start] = *counter;                                                  //������������  
    (*counter)++;                                                           //��������һ  
    for (EdgeNode *p = G->vexlist[start].firstedge; p != NULL; p = p->next) //�������б߱�ڵ�  
    {  
        if (visited[p->adjvex] == false) //��û����  
        {  
            t2.parent[p->adjvex] = start;  
            DFS_ADJ_Recur(G, p->adjvex, visited, dfn, counter);  
        }  
    }  
}  
  
int *DFSTraverse_ADJ_Recur(UDgraph_ADJ G) // ����ݹ飬���ڽӱ� ������ͬ��  
{  
    int *dfn = (int *)calloc(G->n, sizeof(int));  
    bool visited[MAX];  
    int counter = 0;  
    for (int i = 0; i < MAX; i++)  
        t2.parent[i] = -1;  
    for (int i = 0; i < MAX; i++)  
        visited[i] = false;  
  
    for (int i = 0; i < G->n; i++)  
    {  
        if (!visited[i])  
            DFS_ADJ_Recur(G, i, visited, dfn, &counter);  
    }  
    return dfn;  
}  
  
typedef struct //�ǵݹ��õ���ջ  
{  
    int data[MAX];  
    int top;  
} STACK;  
  
int FirstNextIndex_ED(UDgraph_ED G, int i, bool *visited, STACK *st) /*���ص�һ���ڽӵĽڵ�� 
                                                                         �����ڽӽڵ���ջ*/  
{  
    int ret, flag = 0; //flag��ʾ�Ƿ��ҵ�δ���ʵĵ�һ���ڵ�  
    for (int j = 0; j < G->n; j++)  
    {  
        if (G->edge[i][j] && visited[j] == false)  
        {  
            t3.parent[j] = i; //��������������  
            if (!flag)  
            {  
                ret = j;  
                flag = 1;  
            }  
            else  
            {  
                st->data[++(st->top)] = j;  
            }  
        }  
    }  
    if (flag)  
        return ret;  
    else  
        return -1; //δ�ҵ������� -1  
}  
  
void DFS_ED_NoRecur(UDgraph_ED G, int i, bool *visited, int dfn[], int *counter)  
                                          //��ĳ�ڵ㿪ʼ���ǵݹ�����ڽӾ���  
{  
    STACK *st = (STACK *)malloc(sizeof(STACK));  
    st->top = -1;  
  
    int temp;  
  
    visited[i] = true;  
    printf("%d ", G->vertex[i]);  
    dfn[i] = (*counter)++;  
  
    i = FirstNextIndex_ED(G, i, visited, st);  
    while (st->top != -1 || i != -1) //��ջ���ҷ��ʵ���ͷ������ѭ��  
    {  
        while (i != -1) //ѭ��ֱ����ͷ  
        {  
            visited[i] = true;  
            printf("%d ", G->vertex[i]);  
            dfn[i] = (*counter)++;  
            i = FirstNextIndex_ED(G, i, visited, st);  
        }  
        if (st->top != -1) //���ջ����  
        {  
            do  
                temp = st->data[(st->top)--];  
            while (visited[temp] && st->top != -1); //Ԫ�س�ջ��ֱ����ջ����δ���ʵ�  
  
            if (!visited[temp])   //���ջ����δ���ʵ�,����Ч  
                i = temp;  
        }  
    }  
}  
  
int *DFSTraverse_ED_NoRecur(UDgraph_ED G)    //�ǵݹ�����ڽӾ��󣩣�ϸ��ͬ�ϣ���  
{  
    int *dfn = (int *)calloc(G->n, sizeof(int));  
    int counter = 0;  
    bool visited[MAX];  
    for (int i = 0; i < MAX; i++)  
        t3.parent[i] = -1;  
    for (int i = 0; i < G->n; i++)  
        visited[i] = false;  
  
    for (int i = 0; i < G->n; i++)  
        if (!visited[i])  
            DFS_ED_NoRecur(G, i, visited, dfn, &counter);  
    return dfn;  
}  
  
int FirstNextIndex_ADJ(UDgraph_ADJ G, int i, bool *visited, STACK *st, int *dfn, int *counter)  
                                                                        /*���ص�һ���ڽӵĽڵ�ţ� 
                                                                          �����ʡ� 
                                                                         �����ڽӽڵ���ջ (������ͬ��)*/  
  
{  
    EdgeNode *p = G->vexlist[i].firstedge;  
    int ret;  
    int flag = 0;  
  
    printf("%d ", G->vexlist[i].vertex);  
    visited[i] = true;  
    dfn[i] = *counter;  
    (*counter)++;  
    while (p)  
    {  
        if (!visited[p->adjvex])           
        {  
            t4.parent[p->adjvex] = i;  
            if (!flag)  
            {  
                flag = 1;  
                ret = p->adjvex;  
            }  
            else  
            {  
                st->data[++(st->top)] = p->adjvex;  
            }  
        }  
        p = p->next;  
    }  
  
    if (flag)  
        return ret;  
    else  
        return -1;  
}  
  
int *DFS_ADJ_NoRecur(UDgraph_ADJ G, int i, bool *visited, int dfn[], int *counter)  
                                //��ĳ�ڵ㿪ʼ���ǵݹ�����ڽӱ�  
{  
    STACK *st = (STACK *)malloc(sizeof(STACK));  
    st->top = -1;  
  
    int temp;  
    while (st->top != -1 || i != -1)  
    {  
        while (i != -1)  
            i = FirstNextIndex_ADJ(G, i, visited, st, dfn, counter);  
        if (st->top != -1)  
        {  
            do  
                temp = st->data[(st->top)--];  
            while (visited[temp] && st->top != -1);  
  
            if (!visited[temp])  
                i = temp;  
        }  
    }  
}  
  
int *DFSTraverse_ADJ_NoRecur(UDgraph_ADJ G)      //�ǵݹ�����ڽӱ�  
{  
    int *dfn = (int *)calloc(G->n, sizeof(int));  
    int counter = 0;  
    bool visited[MAX];  
  
    for (int i = 0; i < MAX; i++)  
        t4.parent[i] = -1;  
    for (int i = 0; i < G->n; i++)  
        visited[i] = false;  
  
    for (int i = 0; i < G->n; i++)  
        if (!visited[i])  
            DFS_ADJ_NoRecur(G, i, visited, dfn, &counter);  
    return dfn;  
}  
  
void BFS_ED(UDgraph_ED G, int i, bool *visited, int *bfn, int *counter)  //��ĳ�ڵ㿪ʼ�ȹ㣨�ڽӾ���  
{  
    int Q[MAX];  
    int front = 0, rear = 0;     //��ʼ������  
    Q[rear++] = i;          //�׽ڵ���Ӳ����  
    bfn[i] = *counter;  
    (*counter)++;  
    visited[i] = true;  
    printf("%d ", G->vertex[i]);  
  
    while (front != rear)   //ѭ�����ӿ�  
    {  
        i = Q[front++];  //����  
        for (int j = 0; j < G->n; j++)  
        {  
            if (G->edge[i][j] && visited[j] == false) //���ڽ���δ���ʣ�����Ӳ����  
            {  
                t5.parent[j] = i;  
                Q[rear++] = j;  
                bfn[j] = *counter;  
                (*counter)++;  
                visited[j] = true;  
                printf("%d ", G->vertex[j]);  
            }  
        }  
    }  
}  
  
int *BFSTraverse_ED(UDgraph_ED G)  //�ȹ㣨�ڽӾ���  
{  
    int *bfn = (int *)calloc(G->n, sizeof(int));  
    ;  
    int counter = 0;  
    bool visited[MAX];  
  
    for (int i = 0; i < MAX; i++)  
        t5.parent[i] = -1;  
  
    for (int i = 0; i < G->n; i++)  
        visited[i] = false;  
  
    for (int i = 0; i < G->n; i++)  
        if (!visited[i])  
            BFS_ED(G, i, visited, bfn, &counter);  
    return bfn;  
}  
  
void BFS_ADJ(UDgraph_ADJ G, int i, bool *visited, int *bfn, int *counter)//��ĳ�㿪ʼ�ȹ㣨�ڽӱ���ԭ��ͬ�ϣ�  
{  
    int Q[MAX];  
    int rear = 0, front = 0;  
    EdgeNode *p;  
  
    Q[rear++] = i;  
    printf("%d ", G->vexlist[i].vertex);  
    visited[i] = true;  
    bfn[i] = *counter;  
    (*counter)++;  
  
    while (rear != front)  
    {  
        i = Q[front++];  
        p = G->vexlist[i].firstedge;  
        while (p)  
        {  
            if (!visited[p->adjvex])  
            {  
                t6.parent[p->adjvex] = i;  
                Q[rear++] = p->adjvex;  
                printf("%d ", G->vexlist[p->adjvex].vertex);  
                visited[p->adjvex] = true;  
                bfn[p->adjvex] = *counter;  
                (*counter)++;  
            }  
            p = p->next;  
        }  
    }  
}  
  
int *BFSTraverse_ADJ(UDgraph_ADJ G)   //�ȹ㣨�ڽӱ�  
{  
    int *bfn = (int *)calloc(G->n, sizeof(int));  
    bool visited[MAX];  
    int counter = 0;  
  
    for (int i = 0; i < MAX; i++)  
        t6.parent[i] = -1;  
  
    for (int i = 0; i < G->n; i++)  
        visited[i] = false;  
  
    for (int i = 0; i < G->n; i++)  
        if (!visited[i])  
            BFS_ADJ(G, i, visited, bfn, &counter);  
    return bfn;  
}  
  
void printTree(Tree t, int num)   //��ӡ����㣩��  
{  
    printf("index\tparent\n");  
    for (int i = 0; i < num; i++)  
    {  
        printf("%d\t%d\n", i, t.parent[i]);  
    }  
}  
  
void printDfnBfn(int *fn, int num) //��ӡ���\���������  
{  
    for (int i = 0; i < num; i++)  
        printf("%d ", fn[i]);  
    printf("\n");  
}  
  
int main()  
{  
    UDgraph_ADJ G_adj = build_ADJ(".\\input_ADJ.txt");  
    UDgraph_ED G_ed = makeNull_ED();  
    ADJ2ED(G_ed, G_adj);  
  
    // UDgraph_ED G_ed = build_ED("F:\\data_structure\\Lab\\lab3\\input_ED.txt");  
    // UDgraph_ADJ G_adj = makeNull_ADJ();  
    // ED2ADJ(G_adj, G_ed);  
  
    printf("�ڽӾ���\n");  
    printEdge(G_ed);  
    printf("\n\n");  
  
    printf("�ڽӾ������ѣ��ǵݹ飺\n");  
    int *dfn_ed_recur = DFSTraverse_ED_Recur(G_ed);  
    printf("\n");  
    printf("dfn[]:\n");  
    printDfnBfn(dfn_ed_recur, G_ed->n);  
    printTree(t1, G_ed->n);  
    printf("\n");  
  
    printf("�ڽӱ����ѣ��ǵݹ飺\n");  
    int *dfn_adj_recur = DFSTraverse_ADJ_Recur(G_adj);  
    printf("\n");  
    printf("dfn[]:\n");  
    printDfnBfn(dfn_adj_recur, G_adj->n);  
    printTree(t2, G_adj->n);  
    printf("\n");  
  
    printf("�ڽӾ������ѣ��ݹ飺\n");  
    int *dfn_ed_Norecur = DFSTraverse_ED_NoRecur(G_ed);  
    printf("\n");  
    printf("dfn[]:\n");  
    printDfnBfn(dfn_ed_Norecur, G_ed->n);  
    printTree(t3, G_ed->n);  
    printf("\n");  
  
    printf("�ڽӱ����ѣ��ݹ飺\n");  
    int *dfn_adj_Norecur = DFSTraverse_ADJ_NoRecur(G_adj);  
    printf("\n");  
    printf("dfn[]:\n");  
    printDfnBfn(dfn_adj_Norecur, G_adj->n);  
    printTree(t4, G_adj->n);  
    printf("\n");  
  
    printf("�ڽӾ��󣬹��ѣ�\n");  
    int *bfn_ed = BFSTraverse_ED(G_ed);  
    printf("\n");  
    printf("bfn[]:\n");  
    printDfnBfn(bfn_ed, G_ed->n);  
    printTree(t5, G_ed->n);  
    printf("\n");  
  
    printf("�ڽӱ����ѣ�\n");  
    int *bfn_adj = BFSTraverse_ADJ(G_adj);  
    printf("\n");  
    printf("bfn[]:\n");  
    printDfnBfn(bfn_adj, G_adj->n);  
    printTree(t6, G_adj->n);  
    printf("\n");  
  
    getchar();  
    return 0;  
}  

