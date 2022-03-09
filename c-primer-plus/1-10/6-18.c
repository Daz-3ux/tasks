#include<stdio.h>
#define DB 150

int main(int argc, char **argv)
{
    int n = 5;
    int cnt = 1;
    do
    {
        n -= cnt;
        n *= 2;
        printf("In No.%d week, DR.db has %d friends\n", cnt, n);
        cnt++;
    }while(n < DB);


    return 0;
}