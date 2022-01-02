#include<stdio.h>

double litter(double x, double y);

int main(int argc, char **argv)
{
    double x, y;
    scanf("%lf %lf", &x, &y);
    double i = litter(x, y);
    printf("%.2f", i);

    return 0;
}

double litter(double x, double y)
{
    return (x<y)?x:y;
}