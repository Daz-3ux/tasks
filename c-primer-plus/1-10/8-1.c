#include<stdio.h>

int main(int argc, char **argv)
{
    int ch;
    int cnt = 0;

    while((ch = getchar()) != EOF){
        cnt++;
    }

    printf("%d\n", cnt);

    return 0;
}