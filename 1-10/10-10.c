#include<stdio.h>

#define LEN 4

void sum(int x[],int y[],int z[],int n);
void show(int x[],int n);

int main(int argc, char **argv)
{
    int num1[LEN] = {2,4,5,8};
    int num2[LEN] = {1,0,4,6};
    int num3[LEN] = {0};

    sum(num1,num2,num3,LEN);
    show(num3,LEN);

    return 0;
}

void sum(int x[],int y[],int z[],int n)
{
    for(int i=0; i<LEN; i++){
        z[i] = x[i] + y[i];
    }
}

void show(int x[],int n)
{
    for(int i = 0; i < LEN; i++){
        printf("%d\t",x[i]);
    }
}