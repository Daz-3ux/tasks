#include<stdio.h>

int main(int argc, char **argv)
{
    float i , j;
    while(scanf("%f %f", &i, &j) == 2)
    {
        printf("%.3f",i*j);
    }

    return 0;
}