#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    scanf("%d",&n);
    if(n >= 1 && n<= 127){
    printf("%d is %c in ASCII\n",n,n);
    }else{ 
        printf("ERROR\n");
    }

    return 0;
}