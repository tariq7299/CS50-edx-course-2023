#include <cs50.h>
#include <stdio.h>

int collatz(int n);

int main(void)
{

    int n = get_int("Type a number: ");
    int steps = collatz(n);
    printf("%d\n", steps);
}

int collatz(int n)

{
    if (n == 1)
    {
            return 0;
    }
    else if ((n % 2) == 0)
    {
        n = n / 2;
        return 1 + collatz(n);

        /* or

        return 1 + collatz(n / 2) */
    }
    else
    {
        n = 3 * n + 1;
        return 1 + collatz(n);

        /* or

         return 1+ collatz(3 * n + 1;) */
    }

}