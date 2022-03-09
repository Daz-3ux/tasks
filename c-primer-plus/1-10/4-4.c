#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    float x = 0;
    
    scanf("%d", &n);
    if( n>0 ){
        x = n / 100.00;
        printf("%.2f\n", x);
    }

    return 0;
}