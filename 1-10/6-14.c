#include<stdio.h>
#define N 8
int main(int argc, char **argv)
{
    double n1[N];
    double n2[N];

    for (int i = 0; i < N; i++){
        scanf("%lf", &n1[i]);
        for(int j = 0; j <= i; j++){
            n2[i] += n1[j];
        }
    }
    
    for(int i = 0; i < N; i++){
        printf("%f %f\n",n1[i], n2[i]);
    }


}