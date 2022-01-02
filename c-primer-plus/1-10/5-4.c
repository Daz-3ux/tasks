#include <stdio.h>
#define CM_PER_INCH 2.54f
#define CM_PER_FEET 30.48f

int main(void)
{
    int feet;
    float cm, inch;
    scanf("%f", &cm);
    
    while (cm > 0)
    {
        feet = (int)(cm / CM_PER_FEET);
        inch = (cm - CM_PER_FEET * feet) / CM_PER_INCH;
        printf("%.1f cm = %d feet, %.1f inches\n", cm, feet, inch);
        scanf("%f", &cm);
    }
    printf("bye\n");

    return 0;
}