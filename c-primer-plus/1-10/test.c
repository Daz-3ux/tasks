#include <stdio.h>
#define GALLON_TO_LITRE 3.785f
#define MILE_TO_KM 1.609f

int main(void)
{
    float range, oil;

    printf("Please input the range you traveled(in mile):");
    scanf("%f", &range);
    printf("Please input the oil you spend(in gallon):");
    scanf("%f", &oil);

    printf("Fuel consumptions:\n");
    printf("In USA, your oil wear is %.1f mile/gallon.\n", range / oil);
    printf("In Europe, your oil wear is ");
    printf("%.1f litre/100km.\n", (oil * GALLON_TO_LITRE) / (range * MILE_TO_KM));

    return 0;
}