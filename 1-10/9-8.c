#include<stdio.h>

void pow(int x,int y);

int main(int argc, char **argv)
{
    int i, j;
    while(scanf("%d %d", &i, &j) == 2){
        pow(i,j);
    }

    return 0;
}

void pow(int x,int y)
{
    if(y == 0 && x == 0){
        if(x == 0){
            x = 1;
            printf("This is a wrong,but i can make it is 1."
                    "Beacuse I'm the king of the computer!\n");
        }
    }else if(y == 0){
        x = 1;
    }

    if(y > 0){
        int i = x;
        for(y;y > 1;y--){
            x *= i;
        }
    }

    printf("%d\n",x);
}