#include <stdio.h>
#include <ctype.h>
#define EXTRA_HOUR 1.5f
#define BASE_TAX 0.15f
#define EXTRA_TAX 0.2f
#define EXCEED_TAX 0.25f
/*抄的csdn，这个题属实没价值*/

int show_menu(void);
void show_salary(float base_salary, float hours);
int get_choice(void);
void eatline(void);

int main(void)
{
    int ch;
    float n;

    while ((ch = show_menu()) != 5)
    {
        printf("Enter the working hours a week: ");
        while (scanf("%f", &n) != 1 || (n < 0))
        {
            eatline();
            printf("Enter a positive number: ");
        }
        eatline();
        switch (ch)
        {
        case 1:
        {
            show_salary(8.75f, n);
            break;
        }
        case 2:
        {
            show_salary(9.33f, n);
            break;
        }
        case 3:
        {
            show_salary(10.00f, n);
            break;
        }
        case 4:
        {
            show_salary(11.20f, n);
            break;
        }
        }
        putchar('\n');
    }
    printf("Done!\n");

    return 0;
}

int get_choice(void)
{
    int ch;

    scanf("%d", &ch);
    eatline();
    return ch;
}

void eatline(void)
{
    while (getchar() != '\n')
        continue;
    return;
}

int show_menu(void)
{
    int ch;

    printf("**********************************************************************\n");
    printf("Enter the number corresponding to the desired pay rate or action:\n");
    printf("1) $8.75/hr                         2) $9.33/hr\n");
    printf("3) $10.00/hr                        4) $11.20/hr\n");
    printf("5) Quit\n");
    printf("**********************************************************************\n");
    printf("Please you choose: ");
    ch = get_choice();

    while (ch != 1 && ch != 2 && ch != 3 && ch != 4 && ch != 5)
    {
        printf("Please enter 1, 2, 3, 4 or 5: ");
        ch = get_choice();
    }
    return ch;
}

void show_salary(float base_salary, float hours)
{
    float salary, tax, taxed_salary;

    if (hours <= 30)
    {
        salary = hours * base_salary;
        tax = salary * BASE_TAX;
        taxed_salary = salary - tax;
    }
    else if (hours <= 40)
    {
        salary = hours * base_salary;
        tax = 300 * BASE_TAX + (salary - 300) * EXTRA_TAX;
        taxed_salary = salary - tax;
    }
    else
    {
        salary = (40 + (hours - 40) * EXTRA_HOUR) * base_salary;
        if (salary <= 450)
        {
            tax = 300 * BASE_TAX + (salary - 300) * EXTRA_TAX;
        }
        else
        {
            tax = 300 * BASE_TAX + 150 * EXTRA_TAX + (salary - 450) * EXCEED_TAX;
        }
        taxed_salary = salary - tax;
    }
    printf("salary(before tax): $%g\n", salary);
    printf("tax: $%g\n", tax);
    printf("salary(after tax): $%g\n", taxed_salary);
    return;
}
