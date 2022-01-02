#include<stdio.h>

#define FIRST_TAX 0.15
#define SECOND_TAX 0.20
#define LAST_TAX 0.25
#define BASIC_WAGE 1000
#define GRDZBJ 40//狗日的资本家
#define LDBHF 1.5//劳动保护法

int main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);
    float i = 0;
    float j = 0;
    if(n > GRDZBJ){
        i = n - GRDZBJ;
        i *= LDBHF;
        j = LDBHF + i;
    }{
        j = n;
    }
    
    float wage = j * BASIC_WAGE;
    
    float tax = 0;
    if(wage >= 0 && wage <= 300){
        tax = wage * FIRST_TAX;
    }else if(wage >= 300 && wage <= 450){
        tax = 300 * FIRST_TAX + (wage - 300) * SECOND_TAX; 
    }else if(wage > 450){
        tax = 300 * FIRST_TAX + 150 * SECOND_TAX + (wage - 450) * LAST_TAX;
    }

    float sum = wage - tax;

    printf("%.2f %.2f %.2f", wage, tax, sum);

    return 0;
}