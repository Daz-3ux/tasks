#include<stdio.h>
#define LEN 5

void big(double big[],int n);

int main(int argc, char **argv)
{
    double mos[LEN] = {1.1,2.1,66.1,4.1,5.1};

    big(mos,LEN);

    return 0;
}

void big(double big[], int n)
{
    double res[LEN];
    int k = 0;
    for(int i = LEN - 1; i >= 0, k < LEN; i--, k++){
        res[i] = big[k];
    }

    for(int i = 0; i < LEN; i++){
        printf("%f ",res[i]);
    }
}