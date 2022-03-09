#include<stdio.h>

int main(int argc, char **argv)
{
    printf("Please input your first name and then input your last name\n");
    char first_name[100];
    char last_name[100];
    scanf("%s", first_name);
    scanf("%s", last_name);
    printf("Now i konw your name!\nIt's %s %s\n", first_name, last_name);
    
    return 0;
}