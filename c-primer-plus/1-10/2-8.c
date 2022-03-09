#include<stdio.h>

void one_three();
void two();


int main(int argc, char **argv)
{
    printf("Start now!\n");
    one_three();
    printf("Done!\n");
}

void one_three(){
    printf("One\n");
    two();
    printf("Three\n");
}

void two(){
    printf("Two\n");
}