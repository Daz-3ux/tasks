#include<stdio.h>
#include<stdlib.h>
#define MAX 100

int main(int argc, char **argv)
{
    int num[MAX];
    for (int i = 0; i <MAX; i++){
        num[i] = rand()%10;
    }
    int temp;
    for(int i = 0; i < MAX - 1;i++){
        for(int j = 0; j < MAX - i - 1; j++){
            if(num[j] < num[j+1]){
                temp = num[j];
                num[j] = num[j+1];
                num[j+1] = temp;
            }
    }
}

    for(int i = 0; i < MAX; i++){
        printf("%d\t", num[i]);
    }
    return 0;
}