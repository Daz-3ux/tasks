#include <stdio.h>

int main(void)
{
    int i, n;

    printf("Please enter a number (<= 0 to quit): ");
    while ((scanf("%d", &n) == 1) && (n > 0))
    {
        double sum1 = 0.0;
        double sum2 = 0.0;
        for (i = 1; i <= n; i++)
        {
            sum1 += 1.0 / i;
            if (i % 2 == 1)
            {
                sum2 += 1.0 / i;
            }
            else
            {
                sum2 -= 1.0 / i;
            }
        }
        printf("1.0 + 1.0/2.0 + 1.0/3.0 + 1.0/4.0 + ...");
        printf("sum are %f\n",sum1);
        printf("1.0 - 1.0/2.0 + 1.0/3.0 - 1.0/4.0 + ...");
        printf("sum are %f\n",sum2);
        printf("The sum of the first %d items of the two sequences is %g\n", n, sum1 + sum2);
        printf("\nYou can enter again (<= 0 to quit): ");
    }
    printf("Done.\n");

    return 0;
}
