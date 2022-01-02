#include<stdio.h>

void chline(char ch, int i ,int j);//j行i列

int main(int argc, char **argv)
{
    char ch;
    int j,i;
    scanf("%c %d %d",&ch,&j, &i);

    chline(ch, i ,j);

    return 0;
}

void chline(char ch, int i ,int j)
{
    for(int l = 0;l < j; l++){
        for(int m = 0;m < i;m++){
            putchar(ch);
        }
        printf("\n");
    }
}