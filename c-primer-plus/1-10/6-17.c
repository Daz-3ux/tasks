#include<stdio.h>
#define TAX 0.08

int main(int argc, char **argv)
{
    int cnt = 0;
    double money = 100.0;
    do
    {
        money += money * TAX;
        money -= 10;
        cnt++;
    }while(money > 9);
    printf("%d\n",cnt);

    return 0;
}