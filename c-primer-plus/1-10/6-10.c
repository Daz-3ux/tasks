#include<stdio.h>

int main(int argc, char **argv)
{
    int i, j;
    scanf("%d %d", &i, &j);
    int sum = 0;
    int k = i;
    if(j > i){
        for (i; i <= j; i++){
            sum += i * i;
        }
        printf("The sum of the squares from %d to %d is %d", k*k, j*j, sum);
    }else{
        printf("Done");
    }

    return 0;
}