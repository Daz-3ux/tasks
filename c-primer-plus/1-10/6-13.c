#include<stdio.h>
#define N 8

int main(int argc, char **argv)
{
    int num[N];
    int j = 2;
    for (int i = 0; i < N; i++){
        num[i] = j;
        j *= 2;
    }

    int k = 0;
    do{
        printf("%d\n", num[k]);
        k++;
    }while(k < N);

    return 0;
}