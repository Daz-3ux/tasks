#include <stdio.h>

int main(void)
{
    int i, j;
    char ch;
    char c = 'A';

    printf("Please enter a uppercase letter: ");
    scanf("%c", &ch);

    int length = ch - c;//循环次数;
    
    printf("The pyramid of %c is:\n", ch);
    for (i = 0; i <= length; i++)//为全局考量
    {
        char t = 'A' - 1;
        for (j = 0; j < length - i; j++)//左侧空格数;
        {
            printf(" ");
        }
        
        for (j = 0; j <= i; j++)//递增字母;
        {
            printf("%c", ++t);
        }

        for (j = 0; j < i; j++)//递减字母
        {
            printf("%c", --t);
        }
        printf("\n");
    }

    return 0;
}
