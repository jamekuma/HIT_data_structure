#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define LEN 100000

typedef struct st_BST
{
    int key;
    struct st_BST *lchild;
    struct st_BST *rchild;
} BST_NODE;

typedef BST_NODE *BST;
typedef BST_NODE *pos;

void init_array(int A[], int len) // init
{
    for (int i = 0; i < len; i++)
        A[i] = 2 * i + 1;
}

void random_array(int A[], int len)
{
    srand(time(NULL));

    for (int i = len - 1; i > 0; i--)
    {
        int index = rand() % i;

        int temp = A[i];
        A[i] = A[index];
        A[index] = temp;
    }
}

int binSearch(int tar, int A[], int len)
{
    int down = 0;
    int up = len - 1;
    int mid;
    while (up >= down)
    {
        mid = (up + down) / 2; // choose the middle one
        if (A[mid] == tar)
            return mid;
        else if (A[mid] > tar)
            up = mid - 1;
        else
            down = mid + 1;
    }
    return -1; // not found
}

long binSearch_count(int tar, int A[], int len)
{
    int mid;
    int counter = 0;
    int down = 0, up = len - 1;
    while (down <= up)
    {
        mid = (down + up) / 2;
        counter++;
        if (A[mid] == tar)
            return counter;
        else if (A[mid] > tar)
            up = mid - 1;
        else
            down = mid + 1;
    }
    return counter;
}

double ASL_binS_success(int A[], int len)
{
    long res = 0;
    for (int i = 0; i < len; i++)
    {
        res += binSearch_count(A[i], A, len);
    }
    return (double)res / len;
}

double ASL_binS_fail(int A[], int len)
{
    long res = 0;

    res = binSearch_count(0, A, len);
    for (int i = 0; i < len; i++)
    {
        res += binSearch_count(A[i] + 1, A, len);
    }
    return (double)res / (len + 1);
}

BST insert_BST(BST tree, int x)
{
    if (tree == NULL) // tree is empty
    {
        tree = (BST)malloc(sizeof(BST_NODE));
        tree->key = x;
        tree->lchild = NULL;
        tree->rchild = NULL;
    }
    else
    {
        pos p, q, r;
        p = tree;
        while (p != NULL)
        {
            q = p;
            if (x > p->key) // bigger
                p = p->rchild;
            else if (x < p->key) // smaller
                p = p->lchild;
            else // already exist
            {
                printf("%d is already exist in the tree!\n");
                return tree;
            }
        }

        r = (BST)malloc(sizeof(BST_NODE));
        r->key = x;
        r->lchild = NULL;
        r->rchild = NULL; // create new node

        if (x > q->key)
            q->rchild = r;
        if (x < q->key)
            q->lchild = r; // add it to the tree
    }
    return tree;
}

BST createBST(int A[], int len)
{
    BST tree = NULL;
    for (int i = 0; i < len; i++) // insert circularly
    {
        tree = insert_BST(tree, A[i]);
    }
    return tree;
}

pos searchBST(BST tree, int tar)
{
    pos p = tree;
    while (p != NULL) // search until leave node
    {
        if (p->key == tar) // find it
            return p;
        else if (tar > p->key) // bigger
            p = p->rchild;
        else // smaller
            p = p->lchild;
    }
    return NULL;
}

BST deleteNode(BST tree, pos p, pos q)
{
    bool isRoot = false;
    if (p == tree) //if delete root
        isRoot = true;

    if (p->rchild == NULL && p->lchild == NULL) //if p is a leaf
    {
        if (isRoot)
            tree = NULL;
        else if (p == q->lchild)
            q->lchild = NULL;
        else if (p == q->rchild)
            q->rchild = NULL;
        free(p);
    }
    else if (p->rchild == NULL) //rchild is NULL
    {

        if (isRoot)
            tree = p->lchild;
        else if (p == q->lchild)
            q->lchild = p->lchild;
        else if (p == q->rchild)
            q->rchild = p->lchild;

        free(p);
    }
    else if (p->lchild == NULL) // lchild is NULL
    {
        if (isRoot)
            tree = p->rchild;
        else if (p == q->lchild)
            q->lchild = p->rchild;
        else if (p == q->rchild)
            q->rchild = p->rchild;
        free(p);
    }
    else //if p has two children
    {
        if (p->rchild->lchild != NULL) //p's right - left is not NULL
        {
            pos tp, tq;
            tq = p->rchild;
            tp = p->rchild->lchild;
            while (tp->lchild != NULL)
            {
                tq = tp;
                tp = tp->lchild;
            } // find the min
            p->key = tp->key;
            // tree = deleteNode(tree, tp, tq);
            tq->lchild = tp->rchild;
            free(tp);
        }
        else //p's right - left is NULL
        {
            if (isRoot)
                tree = p->rchild;
            else if (p == q->lchild)
                q->lchild = p->rchild;
            else if (p == q->rchild)
                q->rchild = p->rchild;
            p->rchild->lchild = p->lchild;
            free(p);
        }
    }
    return tree;
}

BST delete_BST(BST tree, int tar)
{
    pos p = tree;
    pos q = NULL;
    while (p != NULL) // search until leave node
    {
        if (p->key == tar) // find it
            break;

        else if (tar > p->key) // bigger
        {
            q = p;
            p = p->rchild;
        }
        else // smaller
        {
            q = p;
            p = p->lchild;
        }
    }
    if (p != NULL)
        tree = deleteNode(tree, p, q);
    else
        printf("not find!\n");
    return tree;
}

int searchBST_count_suc(BST tree, int tar) // search for "tar", count the comparative times
{
    int counter = 0;
    pos p = tree;
    while (p)
    {
        counter++;
        if (tar == p->key)
            return counter;
        else if (tar < p->key)
            p = p->lchild;
        else
            p = p->rchild;
    }
    return -1; // error
}

double ASL_BST_success(BST tree, int A[], int len) // figure out the ASL when success
{
    long res = 0;
    for (int i = 0; i < len; i++)
    {
        res += searchBST_count_suc(tree, A[i]);
    }
    return ((double)res / len);
}

long searchBST_count_fail(BST tree, int n) // when arrive the "tree", already compare "n" times
{
    if (tree == NULL) // "tree" is the failed node (NULL pointer), has compared n times
        return n;
    else // "tree" isn't the failed node, accumulate its children's comparative times
        return searchBST_count_fail(tree->lchild, n + 1) + searchBST_count_fail(tree->rchild, n + 1);
}

double ASL_BST_fail(BST tree, int len)
{
    long res = 0;
    res = searchBST_count_fail(tree, 0); // when the "tree" is root, has already compared 0 times
    return ((double)res / (len + 1));    // NULL nodes = ALL nodes + 1
}

int count = 0;
void traverseBST(BST tree, int A[])
{
    if (tree != NULL)
    {
        traverseBST(tree->lchild, A);
        A[count++] = tree->key;
        traverseBST(tree->rchild, A);
    }
}

int main()
{
    /*initialize the data*/
    int rand_A[LEN], sort_A[LEN], out_A[LEN], delete_A[LEN];
    init_array(sort_A, LEN);
    memcpy(rand_A, sort_A, sizeof(int) * LEN);
    random_array(rand_A, LEN);

    /*create the tree*/
    BST tree_rand = createBST(rand_A, LEN);
    BST tree_sort = createBST(sort_A, LEN);

    pos res;
    int tar;
    printf("what value do you want to search?\n");
    scanf("%d", &tar);
    if (res = searchBST(tree_sort, tar))
        printf("Find the node %d\n", res->key);
    else
        printf("Can not found!\n");
    printf("\n");

    printf("tree_sort : ASL(success) = %lf\n", ASL_BST_success(tree_sort, sort_A, LEN));
    printf("tree_sort : ASL(fail) = %lf\n", ASL_BST_fail(tree_sort, LEN));
    printf("tree_rand : ASL(success) = %lf\n", ASL_BST_success(tree_rand, rand_A, LEN));
    printf("tree_rand : ASL(fail) = %lf\n", ASL_BST_fail(tree_rand, LEN));

    count = 0;
    traverseBST(tree_rand, out_A);
    printf("Binsearch : ASL(success) = %lf\n", ASL_binS_success(out_A, LEN));
    printf("Binsearch : ASL(fail) = %lf\n", ASL_binS_fail(out_A, LEN));

    /*delete*/

    printf("what value do you want to delete?\n");
    scanf("%d", &tar);
    tree_rand = delete_BST(tree_rand, tar);
    count = 0;
    traverseBST(tree_rand, delete_A);

    printf("\nafter delete:\n");
    for (int i = 0; i < LEN - 1; i++)
        printf("%d ", delete_A[i]);
    printf("\n");

    while(getchar()=='\n');

    return 0;
}
