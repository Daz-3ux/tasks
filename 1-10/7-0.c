#include <stdio.h>
#define BASE_SALARY 10.0f
#define EXTRA_HOUR 1.5f
#define BASE_TAX 0.15f
#define EXTRA_TAX 0.2f
#define EXCEED_TAX 0.25f

int main(void)
{
    float hours = 0.0f;
    float salary, tax, taxed_salary;

    printf("Enter the working hours a week: ");
    while (scanf("%f", &hours) != 1 || (hours < 0))
    {
        while (getchar() != '\n')
            continue;
        printf("Please enter a positive number: ");
    }
    if (hours <= 30)
    {
        salary = hours * BASE_SALARY;
        tax = salary * BASE_TAX;
        taxed_salary = salary - tax;
    }
    else if (hours <= 40)
    {
        salary = hours * BASE_SALARY;
        tax = 300 * BASE_TAX + (salary - 300) * EXTRA_TAX;
        taxed_salary = salary - tax;
    }
    else
    {
        salary = (40 + (hours - 40) * EXTRA_HOUR) * BASE_SALARY;
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

    return 0;
}