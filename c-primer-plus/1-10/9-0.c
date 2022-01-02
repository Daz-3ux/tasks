#include<stdio.h>

int main()
{
    int cnt = 0;
    long long num[100];
    int n = 0;
    
    for(int i = 0;i < 100; i++){
        scanf("%lld",&num[i]);
        if(num[i] == 0){
            n = i;        
            break;
        }
    }
    
    for(;n > 0; n--){
        if(n != 1){
            printf("%lld ",num[n-1]);
        }else { 
            printf("%lld",num[0]);
        }
    }
    
    return 0;
}