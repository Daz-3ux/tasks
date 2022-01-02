#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);
    int i = 1;
    int j = 1;
    int sum = 0;

    while(i <= n){
        sum += j;
        i++;
        j = i * i;
    }
    printf("%d\n", sum);

    return 0;
}