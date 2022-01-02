#include<stdio.h>
#include<string.h>

int main(int argc, char **argv)
{
    printf("Please input your first name and then input your last name\n");
    char first_name[100];
    char last_name[100];
    scanf("%s ", first_name);
    scanf("%s", last_name);
    printf("\"%20s %20s\"\n", first_name, last_name);
    printf("\"%-20s %-20s\"\n", first_name, last_name);
    printf("\"%*s %*s\"\n",(int)strlen(first_name)+3,first_name,(int)strlen(last_name)+3,last_name);

    
    return 0;
}