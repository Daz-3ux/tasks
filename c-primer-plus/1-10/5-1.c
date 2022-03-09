/*1. 编写一个程序。将用分钟表示的时间转换成以小时和分钟表示的时间。
使用#define或者const来创建一个代表60的符号常量。
使用while循环来允许用户重复键入值，并且当键入一个小于等于0的时间时终止循环。*/

#include<stdio.h>
#define HOUR 60

int main(int argc, char* argv[])
{
    int n;
    scanf("%d", &n);

    while(n > 0){
        if(n>60){
            int i = n / HOUR;
            int j = n % HOUR;
            printf("%02dh%02dm",i,j);
        }else if(n<60){
            printf("0h%02dm",n);
        }
        scanf("%d", &n);     
    }
    return 0;
}