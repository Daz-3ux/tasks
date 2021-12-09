#include<stdio.h>

double big(double big[]);

int main(int argc, char **argv)
{
    double mos[5] = {1.1,2.1,66.1,4.1,5.1};

    double b = big(mos);
    printf("%f\n", b);

    return 0;
}

double big(double big[])
{
    int max = big[0];
    int min = big[0];
    for (int i = 0; i < 5; i++){
        if(*(big + i) > max){
            max = *(big + i);
        }
    }

    for (int i = 0; i < 5; i++){
        if(*(big + i) < min){
            min = *(big + i);
        }
    }

    return max - min;
}