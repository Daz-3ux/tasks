#include<stdio.h>
#include<string.h>

int main(int argc, char **argv)
{
    char s[100];
    scanf("%s",s);
    //printf("%s",s);
    int i = strlen(s);
    //printf("%d\n",i);
    for(i-1;i>=0;i--){
        printf("%c",s[i]);
    }
    printf("\n");

    return 0;
}