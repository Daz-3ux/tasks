#include <stdio.h>

int main()
{
    int ch;
    int i = 0;

    printf("Please enter some characters:\n");
    while ((ch = getchar()) != EOF)
    {
        if (i++ == 10)
        {
            putchar('\n');
            i = 1;
        }
        if (ch >= 32) //空格
        {
            printf(" \'%c\' - %3d ", ch, ch);
        }
        else if (ch == '\n')
        {
            printf(" \\n - \\n\n ");
            i = 0;
        }
        else if (ch == '\t')
        {
            printf(" \\t - \\t ");
        }
        else
        {
            printf(" \'%c\' - ^%c ", ch, ch + 64);
        }
    }
    puts("Done.");

    return 0;
}
