#include<stdio.h>

int main(int argc, char **argv)
{
    int cnt = 0;
    char ch;
    while((ch = getchar()) && ch != '#')
    {
        switch(ch){
        case '.':
            printf("!");
            cnt++;
            break;
        case '!':
            printf("!!");
            cnt++;
            break;
        default:
            putchar(ch);
        }
    }

    putchar(ch);
    printf("%d\n", cnt);
}