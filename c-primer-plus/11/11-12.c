#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    int ch, lower, upper, digit;
    int punct, words;
    lower = upper = digit = 0;
    punct = words = 0;
    bool inword = false;

    printf("Please enter some characters (EOF to quit):\n");
    while ((ch = getchar()) != EOF)
    {
        if (islower(ch))
        {
            lower++;
        }
        else if (isupper(ch))
        {
            upper++;
        }
        else if (isdigit(ch))
        {
            digit++;
        }
        else if (ispunct(ch))
        {
            punct++;
        }
        /*最复杂的单词数*/
        if (!isspace(ch) && !inword)//不是空白以及不是词内，不是词内的意思就是刚遇到第一个字母
        {
            inword = true;
            words++;
        }
        if (isspace(ch) && inword)//离开单词后重置inword状态
        {
            inword = false;
        }
    }
    printf("Words: %d\n", words);
    printf("Lower: %d\n", lower);
    printf("Upper: %d\n", upper);
    printf("Digit: %d\n", digit);
    printf("Punct: %d\n", punct);

    return 0;
}
