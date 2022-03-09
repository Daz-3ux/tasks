#include<stdio.h>

int main(int argc, char **argv)
{
    float slavery;

    printf("\aEnter your desired monthly slavery:");
    printf(" $_______\b\b\b\b\b\b\b");

    scanf("%f",&slavery);
    printf("\n\t%.2f a month is $%.2f a year.",slavery,
                slavery * 12.0);

    printf("\rGee!\n");

    return 0;
}