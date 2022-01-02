#include<stdio.h>

static int n = 0;
void pu();

int main(int argc, char **argv)
{
    char ch;
    while((ch = getchar()) != 'a' && ch != '\n'){
        pu();
    }
}

void pu()
{
    n++;
    printf("hello了%d遍了都兄弟\n",n);
}