#include<stdio.h>
#include<string.h>

int main(int argc, char **argv)
{
    char first_name[100];
    char last_name[100];

    scanf("%s ", first_name);
    scanf("%s", last_name);

    printf("%s %s\n", first_name, last_name);
    printf("%*d %*d\n",(int)strlen(first_name), (int)strlen(first_name),(int)strlen(last_name),(int)strlen(last_name));

    printf("%s %s\n", first_name, last_name);
    printf("%-*d %-*d\n",(int)strlen(first_name), (int)strlen(first_name),(int)strlen(last_name),(int)strlen(last_name));

    return 0;
}