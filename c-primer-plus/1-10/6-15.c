#include <stdio.h>
#define LEN 255

int main()
{
    int i = 0;
    char input[LEN];

    printf("Please enter a string:\n");
    do
    {
        scanf("%c", &input[i]);
    } while (input[i] != '\n' && ++i && i < LEN);

    printf("Reversing print the string is:\n");
    for (i--; i >= 0; i--)
    {
        //↑避免打印换行符;
        printf("%c", input[i]);
    }
    printf("\nDone.\n");

    return 0;
}
