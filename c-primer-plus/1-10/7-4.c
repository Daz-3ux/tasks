#include<stdio.h>

int main(int argc, char **argv)
{
    int cnt = 0;
    char ch;
    while((ch = getchar()) && ch != '#')
    {
        if (ch == '.'){
            printf("!");
            cnt++;
        }else if (ch == '!'){
            printf("!!");
            cnt++;
        }
    }

    putchar(ch);
    printf("%d\n", cnt);
}