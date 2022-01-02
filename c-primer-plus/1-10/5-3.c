#include<stdio.h>

int main(int argc, char **argv)
{
    int n;
    scanf("%d", &n);

    while( n > 0){
        int i = n / 7;
        int j = n % 7;
        if(n == 1){
            printf("1 days is 0 week 1 day!");
        }else if(n > 1 && n < 14){
            printf("%d days are %d week %d ", n , i ,j);
            if(j == 1){
                printf("day!\n");
            }else {
                printf("days!\n");
            }
        }else if(n >= 14){
            printf("%d days are %d weeks %d ", n , i , j);
            if(j == 1){
                printf("day!\n");
            }else {
                printf("days!\n");
            }
        }

        scanf("%d", &n);
    }

    return 0;
}