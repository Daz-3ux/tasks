#include<stdio.h>

int main() 
{
    int n = 2147483647;
    printf("%d\n",n + 1);//整数上溢
    float i = 3.402823e38;
    printf("%f\n",i * i);//浮点数上溢
    float j = 1.4e-45;
    printf("%f\n",j / 10.0);//浮点数下溢

    return 0;
}