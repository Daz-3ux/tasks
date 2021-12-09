#include<stdio.h>

int main(int argc, char **argv)
{
    for(char s = 'F';s>='A';s--) {
        for(char c = 'F';c >=s;c--){
            printf("%c", c);
        }
        printf("\n");
    }
}