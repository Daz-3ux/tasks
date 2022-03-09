#include<stdio.h>

void larger_of(double* i, double* j);

int main(int argc, char **argv)
{
    double i,j;
    scanf("%lf %lf", &i, &j);
    printf("%f %f\n",i,j);
    larger_of(&i, &j);
    printf("NOW: %f %f\n",i,j);

    return 0;
}

void larger_of(double* i, double* j)
{
    double big = (*i>*j) ? *i : *j;
    *i = big;
    *j = big;
}

