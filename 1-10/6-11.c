#include<stdio.h>
#define N 8

int main(int argc, char **argv)
{
    int num[N];
    for (int i = 0; i < 8; i++){
        scanf("%d", &num[i]);
    }

    for (int i = N-1; i >= 0; i--){
        printf("%d ",num[i]);
    }

    return 0;
}