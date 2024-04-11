#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int rows, i, space, k;


    do
    {
        rows = get_int("Enter a number: ");
    }
    while (rows <= 0 || rows > 8);

    for (i = 1; i <= rows; ++i)
    {

        for (space = rows; space > i; --space)
        {
            printf(" ");
        }
        for (k = 1; k <= i; ++k)
        {
            printf("#");
        }
        printf("  ");
        for (k = 1; k <= i; ++k)
        {
            printf("#");
        }
        printf("\n");
    }
}