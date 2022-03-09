#include<stdio.h>
#include<ctype.h>
#define STOP '#'

int get_first(void);

int main(int argc, char **argv)
{
    int ch;

    printf("Enter some characters,'#' to quit.\n");
    while ((ch = get_first()) != STOP) {
        printf("Result: %c\n", ch);
        printf("Again,'#' to quit\n");
    }

    printf("Done");

    return 0;
}

int get_first(void)
{
    int ch;

    do
    {
        ch = getchar();
    }while(isspace(ch));//妙啊！！读取第一个非空字符,如果是空格那么将满足while条件然后循环至下一字符
                        //直至字符不是空格为止；

    while(getchar() != '\n'){//消耗后续字符以及\n
        continue;
    }

    return ch;
    
}