#include<stdio.h>
#define CM 2.54

int main(int argc, char **argv)
{
    int n;
    float high = 0.0;
    scanf("%d", &n);
    high = n * CM;
    printf("%.2f",high);

    return 0;
}