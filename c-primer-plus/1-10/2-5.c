#include<stdio.h>

void br();
void ic();

int main(int argc,const char* argv[])
{
    br();
    ic();
    printf("\n");
    ic();
    printf("\n");
    br();

    return 0;
}

void br(){
    printf("Brazil，Russia");
}

void ic(){
    printf("India,Russia");
}