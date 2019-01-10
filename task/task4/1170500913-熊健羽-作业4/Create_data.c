#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SET 100


int main()
{
    freopen(".//data.in", "w", stdout);
    printf("%d\n", SET);
    srand(time(NULL));
    for (int i = 0; i < SET; i++)
    {
        int num = rand() % 100 + 1;
        printf("%d\n", num);
        for (int j = 0; j < num; j++)
        {
            printf("%d ", rand() % 100 + 1);
        }
        printf("\n");
    }
    return 0;
}
