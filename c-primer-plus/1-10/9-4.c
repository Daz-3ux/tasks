#include<stdio.h>

double th(double i, double j);

int main(int argc, char **argv)
{
    double i,j;
    double average = 0;
    scanf("%lf %lf",&i,&j);
    average =  th(i,j);
    printf("%f\n",average);

    return 0;
}

double th(double i, double j)
{
    double l = 1.00/i;
    double m = 1.00/j;
    double sum = (double)(l+m)/2.00;
    double ave = 1.00/sum;

    return ave;
}