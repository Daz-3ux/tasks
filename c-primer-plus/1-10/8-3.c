#include<stdio.h>
#include<ctype.h>

int main(int argc, char **argv)
{
    int ch;
    int cnt_upper = 0;
    int cnt_lower = 0;
    int cnt_other = 0;

    while((ch = getchar()) != EOF)
    {
        if(isupper(ch)){
            cnt_upper++;
        }else if(islower(ch)){
            cnt_lower++;
        }else{
            cnt_other++;
        }
    }
    printf("%d %d %d\n", cnt_upper,cnt_lower,cnt_other);

    return 0;
}