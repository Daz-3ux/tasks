//↑摄氏温度 = 5.0 / 9.0 * (华氏温度 - 32);
//↑开氏温度 = 摄氏温度 + 273.16;

#include<stdio.h>

void Temperature(double);

int main(int argc, char* argv[])
{
    double n;
    scanf("%lf", &n);
    Temperature(n);

    return 0;
}

void Temperature(double n)
{
    double i ,j;
    i = 5.0 / 9.0 * (n - 32);
    j = i + 273.16;
    printf("huashi = %.2f\n",n);
    printf("sheshi = %.2f\n",i);
    printf("kaishi = %.2f\n",j);
}