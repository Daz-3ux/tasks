#include<stdio.h>

int main(int argc, char **argv)
{
    int kg = 0;
    int hh = 0;
    int qt = 0;
    char ch;
    while((ch = getchar()) != '#'){
        if(ch == ' '){
            kg++;
        }else if(ch == '\n'){
            hh++;
        }else {
            qt ++;
        }
    }

    printf("%d %d %d",kg,hh,qt);
}