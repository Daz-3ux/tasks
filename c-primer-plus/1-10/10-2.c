#include <stdio.h>
#define LEN 5

void copy_arr(double x[], const double source[], int n);
void copy_ptr(double *x, const double *source, int n);
void copy_ptrs(double *x, const double *source, const double *end);
void show_arr(const double x[], int n);

int main(void)
{
    double source[LEN] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double target1[LEN];
    double target2[LEN];
    double target3[LEN];

    printf("Source array:\n");
    show_arr(source, LEN);

    copy_arr(target1, source, LEN);
    printf("Target1:\n");
    show_arr(target1, LEN);

    copy_ptr(target2, source, LEN);
    printf("Target2:\n");
    show_arr(target2, LEN);

    copy_ptrs(target3, source, source + LEN);
    printf("Target3:\n");
    show_arr(target3, LEN);

    printf("Done.\n");

    return 0;
}

void show_arr(const double x[], int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%g\t", x[i]);
    }
    putchar('\n');
}

void copy_arr(double x[], const double source[], int n)//数组复制
{
    int i;
    for (i = 0; i < n; i++)
    {
        x[i] = source[i];
    }
}

void copy_ptr(double *x, const double *source, int n)//指针复制
{
    int i;
    for (i = 0; i < n; i++)
    {
        *(x + i) = *(source + i);
    }
}

void copy_ptrs(double *x, const double *source, const double *end)//指针开头结尾
{
    int i;
    for (i = 0; i < end - source/*就这里比较特殊*/; i++)
    {
        *(x + i) = *(source + i);
    }
}
