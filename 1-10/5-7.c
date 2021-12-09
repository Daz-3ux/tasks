#include<stdio.h>

void lf(double);

int main(int argc, char **argv)
{
    double n;
    scanf("%lf", &n);
    lf(n);
}

void lf(double n){
    printf("%f de cube shi %f\n",n,n*n*n);
}