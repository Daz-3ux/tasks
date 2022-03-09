#include<stdio.h>
#define G 3.0e-23
#define g 950

int main(int argc, char **argv)
{
    float n;
    int gramme = 0;
    double number = 0.0;
    scanf("%f",&n);
    gramme = n * g;
    number = gramme / G;
    printf("%lf", number);

    return 0;
}