#include<stdio.h>

int main(int argc, char **argv)
{
    float n;
    scanf("%f", &n);
    int size = 100;//Mb 

    printf("At %.2f megabits per second,a file of %.2f megabytes\n",n,n/8.00);
    printf("downloads in %.2f seconds\n",(float)size/n);

    return 0;
}