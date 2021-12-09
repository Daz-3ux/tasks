#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int main(void)
{
    float count;
    bool inword = false;
    int ch;
    int words, letter;
    words = letter = 0;

    printf("Please enter some words (EOF to quit):\n");
    while ((ch = getchar()) != EOF)
    {
        if (ispunct(ch))
        {
            continue;
        }
        if (isalpha(ch))
        {
            letter++;
        }

        //记录单词数
        //单词开头
        if (!isspace(ch) && !inword)//如果此时不是一个空白字符 以及  inword值为false
        {
            inword = true;
            words++;
        }
        //单词后面的空白
        if (isspace(ch) && inword)//如果是一个空白字符 以及 inword值为true
        {
            inword = false;
        }
    }

    count = (float)letter / words;
    printf("Total words: %d.\n", words);
    printf("Total letters: %d.\n", letter);
    printf("Average letters of words: %g.\n", count);

    return 0;
}
