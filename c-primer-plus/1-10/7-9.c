#include<stdio.h>
#include<math.h>

int main(int argc, char **argv)
{
    int n;
    
    if((scanf("%d", &n) == 1) && n > 0){
        for(int i = 2; i <= n; i++){
            int prime = 1;
            for (int j = 2; j <= sqrt(i); j++)
            {
                if (i % j == 0)
                {
                    prime = 0;
                    break;
                }
            }
            if (prime)
            {
                printf("%-3d", i);
            }
        }
    }
} 
