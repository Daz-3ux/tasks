#include<stdio.h>

void x();

int main(int argc, const char* argv[])
{
    x();
    x();
    x();
    printf("\n");
    x();
    x();
    printf("\n");
    x();
    printf("\n");

    return 0;
}

void x(){
    printf("Smile!");
}
