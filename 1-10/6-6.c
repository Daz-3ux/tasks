#include<stdio.h>

int main(int argc, char **argv)
{
    int i, j;
    scanf("%d %d", &i, &j);

    for(i; i <= j; i++){
        printf("%d %d %d\n", i, i*i, i*i*i);
    }

    return 0;
}