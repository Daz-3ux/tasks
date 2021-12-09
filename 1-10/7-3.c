#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    int even = 0;
    int odd = 0;
    int sum_even = 0;
    int sum_odd = 0;
    float aver_even = 0;
    float aver_odd = 0;

    while((scanf("%d", &n) == 1) && (n != 0)){
        if(n % 2 == 0){
            even++;
            sum_even += n;
        }else if(n % 2 != 0){
            odd++;
            sum_odd += n;
        }
        if(even > 0){
            aver_even = (float)sum_even / even;
        }
        if(odd > 0){
            aver_odd = (float)sum_odd / odd;
        }
    }

    printf("even: %d, odd: %d\n",even,odd);
    printf("aver_even: %f,aver_odd: %f\n",aver_even,aver_odd);


    return 0;
}
