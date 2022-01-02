#include<stdio.h>

int main(int argc, char **argv)
{
    int cnt = 0;
    char ch;
    int n = 0;
    while((ch = getchar()) && ch != '#')
    {
        int i = 0;
        if(ch == 'e'){
            n = 1;
            i = 1;
        }
        if(n == 1 && i == 0){
            if(ch == 'i'){
                cnt++;
                n = 0;
            }else {
                n = 0;
            } 
        }
    }
    printf("%d\n", cnt);
    return 0;
}