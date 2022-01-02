#include<stdio.h>

int main(int argc, char **argv)
{
    //dap 100 0.1
    //dei 100 0.05
    int cnt = 0;
    double dap = 100;
    double dei = 100;
    do
    {
        dap = dap + 10;
        dei = dei + (dei*0.05);
        cnt++;
    }while(dap >= dei);
    printf("%.2f %.2f %d\n", dap, dei, cnt);
}