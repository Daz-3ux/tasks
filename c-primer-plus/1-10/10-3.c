#include<stdio.h>

int big(int big[]);

int main(int argc, char **argv)
{
    int mos[5] = {1,2,66,4,5};

    int b = big(mos);
    printf("%d\n", b);

    return 0;
}

int big(int big[])
{
    int max = big[0];
    for (int i = 0; i < 5; i++){
        if(*(big + i) > max){
            max = *(big + i);
        }
    }

    return max;
}