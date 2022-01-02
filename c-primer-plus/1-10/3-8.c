#include<stdio.h>

int main(int argc, char **argv)
{
    int n;//bs
    scanf("%d", &n);
    printf("%d杯是：\n",n);
    printf("品脱： %.2f\n", (float)n * 0.5);
    printf("盎司： %.2f\n", (float)n * 8);
    printf("汤勺： %.2f\n", (float)n * 16);
    printf("茶勺： %.2f\n", (float)n * 48);

    return 0;
}