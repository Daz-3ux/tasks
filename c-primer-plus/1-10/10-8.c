#include<stdio.h>
#define LEN 7
#define LEN2 3
#define START 2
#define END 4 

void copy(const int a[],int b[],int n, int m);
void show(int s[]);

int main(int argc, char **argv)
{
    const int cc[LEN] = {1,2,3,4,5,6,7};
    int cv[LEN2];
    copy(cc, cv, START, END);
    show(cv);
}

void copy(const int a[],int b[],int n, int m)
{
    for(int i = 0;i < LEN2,n <= m;n++,i++){
        b[i] = a[n];
    }
    return;
}

void show(int s[])
{
    for(int i = 0; i < LEN2; i++){
        printf("%-5d ", s[i]);
    }
}