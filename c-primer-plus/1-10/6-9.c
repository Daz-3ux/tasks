#include<stdio.h>

void hh(float, float);

int main(int argc, char **argv)
{
    float i , j;
    while(scanf("%f %f", &i, &j) == 2)
    {
        hh(i,j);
    }

    return 0;
}

void hh(float i ,float j){
    printf("%.3f",i*j);
}