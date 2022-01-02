#include<stdio.h>
#include<ctype.h>
#define MAX 10//最多输出十个字符

void mygetchar(char str[], int n);

int main(int argc, char** argv)
{
    char input[MAX];

    mygetchar(input,MAX);

    printf("Look:\n");
    puts(input);
    printf("Done\n");
    
    return 0;
}

void mygetchar(char str[], int num)
{
    int i = 0;
    for(i; i < MAX; i++){
        str[i] = getchar();
        if(isspace(str[i])){
            break;
        }
    }
    str[i] = '\0';
}