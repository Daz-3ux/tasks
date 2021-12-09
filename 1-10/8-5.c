#include <stdio.h>
#include <ctype.h>

int main(void)
{
    int high = 100;
    int low = 1;
    int guess = (high + low) / 2;//50
    int response;//ch

    printf("Pick an integer from 1 to 100. I will try to guess ");
    printf("it.\nRespond with a y if my guess is right, with");
    printf("\na h if it is high, and with an l if it is low.\n");
    printf("Uh...is your number %d?\n", guess);
    while ((response = getchar()) != 'y')
    {
        if (response == '\n')
        {
            continue;//回到while
        }
        if (tolower(response) != 'h' && tolower(response) != 'l')
        {
            printf("I don't understand that response. Please enter h for\n");
            printf("high, l for low, or y for correct.\n");
            continue;//回到while
        }
        if (tolower(response) == 'h')
        {
            high = guess - 1;
        }
        else if (tolower(response) == 'l')
        {
            low = guess + 1;
        }
        guess = (high + low) / 2;//if 'h' (49+1)/2 = 25;   if'l'  (100+51)/2 = 75;
                                 //if 'h' (24+1)/2 = 12;   if'l'  (100+76)/2 = 88;
        printf("Well, then, is it %d?\n", guess);
    }
    printf("I knew I could do it!\n");

    return 0;
}
