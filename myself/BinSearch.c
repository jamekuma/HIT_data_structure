#include <stdio.h>

/*ascending sort*/

int binSearch_NoRecur(int tar, int A[], int len)
{
    int down = 0;
    int up = len - 1;
    int mid;
    while (up >= down)
    {
        mid = (up + down) / 2;
        if (A[mid] == tar)
            return mid;
        else if (A[mid] > tar)
            up = mid - 1;
        else
            down = mid + 1;
    }
    return -1;
}

int binSearch_Recur(int tar, int start, int end, int A[])
{
    if (start > end)
        return -1;
    int mid = (start + end) / 2;
    if (A[mid] == tar)
        return mid;
    else if (A[mid] > tar)
        return binSearch_Recur(tar, start, mid - 1, A);
    else
        return binSearch_Recur(tar, mid + 1, end, A);
}

int main()
{
    int array[10] =
        {2, 4, 5, 7, 8, 10, 11, 14, 19, 20};
    int target;
    int res;
    scanf("%d", &target);
    if ((res = binSearch_Recur(target, 0, 9, array)) != -1)
    {
        printf("index is %d\n", res);
    }
    else
        printf("not find\n");
    return 0;
}