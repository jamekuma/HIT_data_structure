#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 1000
int index_BF(char *s, char *t)
{
    int i = 0, j = 0;
    while (i < strlen(s) && j < strlen(t))
    {
        if (s[i] == t[j])
        {
            i++;
            j++;
        }
        else
        {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j == strlen(t))
        return i - strlen(t);
    else
        return -1;
}

void getNext(char *t, int next[])
{
    next[0] = -1;
    for (int i = 1; i < strlen(t); i++)
    {
        int k = next[i - 1];

        while (k != -1 && t[k] != t[i - 1])
        {
            k = next[k];
        }

        next[i] = k + 1;
    }
}

int KMP(char *s, char *t)
{
    int next[MAX];

    getNext(t, next);

    int i = 0, j = 0;
    while (i < strlen(s) && j < strlen(t))
    {
        if (j == -1 || s[i] == t[j])
        {
            i++;
            j++;
        }
        else 
            j = next[j];
    }
    if (j == strlen(t))
        return i - j;
    else
        return -1;
}

int main()
{
    char *s = "ababcabcacbab";
    char *t = "abcac";
    printf("%d", KMP(s, t));
    return 0;
}
