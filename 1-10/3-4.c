#include<stdio.h>
int main()
{
    float n;
    printf("Enter a floating-point value: ");
    scanf("%f",&n);
    printf("fixed-point notation: %.6f\n",n);
    printf("exponential notation: %e\n",n);
    printf("p notion: %a\n",n);


    return 0;
}