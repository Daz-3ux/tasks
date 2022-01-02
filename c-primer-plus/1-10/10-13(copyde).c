#include <stdio.h>
#define ROWS 3
#define COLS 5

void store(double ar[], int n);
double average2d(double ar[][COLS], int rows);
double max2d(double ar[][COLS], int rows);
void showarr2(double ar[][COLS], int rows);
double average(const double ar[], int n);

int main(void)
{
    int row;
    double stuff[ROWS][COLS];

    for (row = 0; row < ROWS; row++)
    {
        printf("Please enter %d numbers for %d row\n", COLS, row + 1);
        store(stuff[row], COLS);
    }
    printf("Array:\n");
    showarr2(stuff, ROWS);

    for (row = 0; row < ROWS; row++)
    {
        printf("Average for row %d is %g.\n", row + 1, average(stuff[row], COLS));
    }
    printf("Average is %g.\n", average2d(stuff, ROWS));
    printf("Maximum is %g.\n", max2d(stuff, ROWS));
    printf("Done.\n");

    return 0;
}

void store(double ar[], int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        printf("Please enter a number: ");
        scanf("%lf", &ar[i]);
    }
    return;
}

double average2d(double ar[][COLS], int rows)
{
    int i, j;
    double sum = 0.0;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            sum += ar[i][j];
        }
    }
    if (rows * COLS > 0)
    {
        return sum / (rows * COLS);
    }
    else
    {
        return 0.0;
    }
}

double max2d(double ar[][COLS], int rows)
{
    int i, j;
    double max = ar[0][0];

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            max = max < ar[i][j] ? ar[i][j] : max;
        }
    }
    return max;
}

void showarr2(double ar[][COLS], int rows)
{
    int i, j;

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            printf("%-5g", ar[i][j]);
        }
        putchar('\n');
    }
    return;
}

double average(const double ar[], int n)
{
    int i;
    double sum = 0.0;

    for (i = 0; i < n; i++)
    {
        sum += ar[i];
    }
    if (n > 0)
    {
        return sum / n;
    }
    else
    {
        return 0.0;
    }
}
