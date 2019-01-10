#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define INF 10000
#define MAX 100

typedef int datatype;

typedef struct GN
{
    int edge[MAX][MAX];
    datatype vertex[MAX];
    int n;
    int e;
} GraphNode;

typedef GraphNode *Graph; //point to a graph

Graph makeNullGraph(void)
{
    Graph G;
    G = (Graph)malloc(sizeof(GraphNode));
    G->n = 0;
    G->e = 0;
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            G->edge[i][j] = INF; //initialize edge matrix
    return G;
}

Graph readGraph(char *path)
{
    FILE *fp;

    Graph G = makeNullGraph();

    if ((fp = fopen(path, "r+")) == NULL) //handle the open error
    {
        printf(path);
        printf(" open failed!\n");
        exit(1);
    }
    char s[MAX];
    int line = 0; //txt's line number
    int counter;
    char *f_ch;
    int start;
    int end;
    int w;
    while (fgets(s, MAX, fp) != NULL)
    {
        line++;
        switch (line)
        {
        case 1:
            if (sscanf(s, "%d", &(G->n)) != 1)
            {
                printf("input error!\n");
                exit(2);
            }
            break;
        case 2:
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
            if (counter != G->n) //if num is mismatching,print warning
                printf("warning: there is node which is not intialized!\n");
            break;
        default:
            if (sscanf(s, "%d~%d %d", &start, &end, &w) == 3)
            {
                if (G->edge[start][end] == INF && start != end)
                {
                    G->edge[start][end] = w;
                    G->edge[end][start] = w;
                    (G->e)++;
                }
            }
            else if (sscanf(s, "%d->%d %d", &start, &end, &w) == 3)
            {
                if (G->edge[start][end] == INF && start != end)
                {
                    G->edge[start][end] = w;
                    (G->e)++;
                }
            }
            else
            {
                printf("edge's input error\n");
                exit(2);
            }
        }
    }
    fclose(fp);

    return G;
}

int findIndexOfMin(int D[], int n, bool S[])
{
    int min;
    int min_index = 0;

    while (S[min_index] && min_index != n) //ignore the item in S
        min_index++;

    if (min_index == n) //if all item is in S
        return -1;
    else //find w in the (V - S), whose D[w] is minmum
    {
        min = D[min_index];
        for (int i = min_index; i < n; i++)
        {
            if (D[i] < min && S[i] == false)
            {
                min = D[i];
                min_index = i;
            }
        }
        return min_index;
    }
}

/*Dijkstra main arithmetic*/
void Dijkstra(Graph G, int start, int D[], int P[], bool S[])
{
    if (start >= G->n || start < 0)
    {
        printf("node is not exist!\n");
        exit(3);
    }

    for (int i = 0; i < G->n; i++) //init D[], P[], S[]
    {
        D[i] = G->edge[start][i];
        P[i] = start;
        if (i == start)
            S[i] = true;
        else
            S[i] = false;
    }

    int w;
    w = findIndexOfMin(D, G->n, S); //find w in the (V - S), whose D[w] is minmum
    while (w != -1)
    {
        S[w] = true;                   //add w into S
        for (int v = 0; v < G->n; v++) // update the D[]
        {
            if (!S[v]) //only update D[v], v is in the S
            {
                if (D[v] > D[w] + G->edge[w][v])
                {
                    D[v] = D[w] + G->edge[w][v]; //choose the smaller
                    P[v] = w;                    //note the trace
                }
            }
        }
        w = findIndexOfMin(D, G->n, S); //continue find w in the (V - S), whose D[w] is minmum
    }
}

/* print the miniway from one sourse to every sourse
 * and its process
 */
void printMinway_OneSourse(Graph G, int start, int *D, int *P)
{
    if (start < 0 || start >= G->n)
    {
        printf("node is not exist!\n");
        return;
    }

    int end;
    int path[MAX]; //temp index of path (stack)
    int top;

    for (end = 0; end < G->n; end++)
    {
        if (end != start) //traverse every node(except start node)
        {
            top = -1; //put stack empty
            int temp = end;
            while (temp != start) //go along with the P[]
            {
                path[++top] = temp; //push the node
                temp = P[temp];
            }
            path[++top] = start;

            if (D[end] != INF)
            {
                printf("v%d to v%d\'s minway is %d :", start, end, D[end]);

                while (top != -1)
                {
                    printf(" v%d ", path[top--]); //pop the node
                    if (top != -1)
                        printf("->");
                }
            }
            else
            {
                printf("v%d can\'t reach v%d!", start, end);
            }
            printf("\n");
        }
    }
}

/*Floyd main arithmetic*/
void Floyd(Graph G, int D[][MAX], int P[][MAX])
{
    for (int i = 0; i < G->n; i++) //initialize D[][], P[][]
    {
        for (int j = 0; j < G->n; j++)
        {
            D[i][j] = G->edge[i][j];
            P[i][j] = -1;
        }
    }

    for (int k = 0; k < G->n; k++)
    {
        for (int i = 0; i < G->n; i++)
        {
            for (int j = 0; j < G->n; j++)
            {
                if (D[i][j] > D[i][k] + D[k][j]) //update D[]
                {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = k; //note the midium node
                }
            }
        }
    }
}

/* print the process according to P[][]
 * in a recursive way
 */
void printProcess(int P[][MAX], int i, int j)
{
    if (P[i][j] == -1)
    {
        printf(" v%d ->", i);
    }
    else
    {
        printProcess(P, i, P[i][j]);
        printProcess(P, P[i][j], j);
    }
}

/* print the miniways from every sourse to every sourse
 * and their process
 */
void printMinway_EveryNode(Graph G, int D[][MAX], int P[][MAX])
{
    for (int i = 0; i < G->n; i++)
    {
        for (int j = 0; j < G->n; j++)
        {
            if (i != j) //traverse every pair of nodes
            {
                if (D[i][j] != INF)
                {
                    printf("v%d to v%d\'s minway is %d :", i, j, D[i][j]); //print minway
                    printProcess(P, i, j);                                 //print process
                    printf(" v%d\n", j);
                }
                else
                {
                    printf("v%d can\'t reach v%d!\n", i, j);
                }
            }
        }
        printf("\n");
    }
}

void printMinway_OneTarget(Graph G, int tar)
{
    if (tar < 0 || tar >= G->n)
    {
        printf("node is not exist!\n");
        return;
    }
    int D[MAX], P[MAX];
    bool S[MAX];
    Graph tmp_G = makeNullGraph(); //store the reverse edge matrix

    tmp_G->e = G->e;
    tmp_G->n = G->n;

    //init temp graph
    for (int i = 0; i < G->n; i++)
        tmp_G->vertex[i] = G->vertex[i];
    for (int i = 0; i < tmp_G->n; i++)
    {
        for (int j = 0; j < tmp_G->n; j++)
        {
            tmp_G->edge[i][j] = G->edge[j][i];
        }
    }

    Dijkstra(tmp_G, tar, D, P, S); //use Dijikstra, see as One-Sourse

    for (int i = 0; i < G->n; i++)
    {
        if (tar != i) //traverse all the node (except itself)
        {
            if (D[i] != INF) //if they are reachable
            {
                printf("v%d to v%d\'s minway is %d :", i, tar, D[i]);
                int k = i;
                while (k != tar) //print the sequence
                {
                    printf(" v%d ->", k);
                    k = P[k];
                }
                printf(" v%d\n", tar);
            }
            else
            {
                printf("v%d can\'t reach v%d!\n", i, tar);
            }
        }
    }

    free(tmp_G);
}

void printMinway_One2One(Graph G, int start, int end)
{
    if (start < 0 || start >= G->n || end < 0 || end >= G->n)
    {
        printf("node is not exist!\n");
        return;
    }

    int D[MAX][MAX], P[MAX][MAX];
    Floyd(G, D, P); //execute Floyd firstly

    if (D[start][end] != INF) //choose the pair of nodes which is wanted
    {
        printf("v%d to v%d\'s minway is %d :", start, end, D[start][end]);
        printProcess(P, start, end);
        printf(" v%d\n", end);
    }
    else
    {
        printf("v%d can\'t reach v%d!\n", start, end);
    }
}


/*print Reachability Matrix*/
void printReachMatrix(Graph G, int D[][MAX])
{
    printf("\nReachability matrix:\n");
    for (int i = 0; i < G->n; i++)
    {
        printf("v%d ", i);
    }
    printf("\n");
    for (int i = 0; i < G->n; i++)
    {
        for (int j = 0; j < G->n; j++)
        {
            if (i != j)
                printf("%d  ", D[i][j] != INF);
            else
                printf("1  ");
        }
        printf(" v%d\n", i);
    }
}

int main()
{
    Graph G;
    G = readGraph(".\\input.txt");

    /*One to others*/
    printf("One to others (Dijkstra):\n");
    int D_di[MAX], P_di[MAX];
    bool S_di[MAX];
    Dijkstra(G, 0, D_di, P_di, S_di);
    printMinway_OneSourse(G, 0, D_di, P_di);

    /*every pair of nodes*/
    printf("\nevery pair of nodes (Floyd):\n");
    int D_fl[MAX][MAX], P_fl[MAX][MAX];
    Floyd(G, D_fl, P_fl);
    printMinway_EveryNode(G, D_fl, P_fl);

    /*others to one*/
    printf("\nothers to one (Dijkstra):\n");
    printMinway_OneTarget(G, 4);

    /*One to One*/
    printf("\nOne to One (Floyd):\n");
    printMinway_One2One(G, 0, 4);

    /*Reachability matrix*/
    printReachMatrix(G, D_fl);

    /*free the graph*/
    free(G);

    getchar();
    return 0;
}