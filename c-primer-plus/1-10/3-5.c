#include<stdio.h>
#define seconds 3.156e7

int main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);
    if(n>=0 && n<=130){
    printf("%f\n",seconds*n);
    }else{
        printf("建国以后不许成精！！！\n");
    }

    return 0;
}