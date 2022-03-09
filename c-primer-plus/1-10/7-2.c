#include<stdio.h>

int main(int argc, char **argv)
{
    int cnt = 0;
    char ch;
    while((ch = getchar()) != '#'){
        cnt++;
        printf("%c %d ", ch, ch);
        if(cnt % 8 == 0){
            printf("\n");
        }
    }


    return 0;
}