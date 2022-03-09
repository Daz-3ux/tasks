#include <stdio.h>
#define TAX 0.08

int main(void)
{
    int i = 0;
    double Chuckie = 100.0;

    do
    {
        i++;
        Chuckie += Chuckie * TAX;
        Chuckie -= 10;
        printf("(%d)account:%g.\n", i, Chuckie);
    } while (Chuckie > 9);
    //↑若余额低于9则不够下一年支取;
    printf("After %d years:\n", ++i);
    printf("Chuckie has taken all of the money!\n");

    return 0;
}
