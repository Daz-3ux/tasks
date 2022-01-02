#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);
    int i = n + 10;
    while(n <= i){
        printf("%d", n);
        n++;
        if(n <= i){
            printf(" ");
        }
    }

    return 0;
}