#include <cs50.h>
#include <stdio.h>

int get_cents(void);
int calculate_quarters(int cents);
int calculate_dimes(int cents);
int calculate_nickels(int cents);
int calculate_pennies(int cents);

int main(void)
{
    // Ask how many cents the customer is owed
    int cents = get_cents();

    // Calculate the number of quarters to give the customer
    int quarters = calculate_quarters(cents);
    cents = cents - quarters * 25;

    // Calculate the number of dimes to give the customer
    int dimes = calculate_dimes(cents);
    cents = cents - dimes * 10;

    // Calculate the number of nickels to give the customer
    int nickels = calculate_nickels(cents);
    cents = cents - nickels * 5;

    // Calculate the number of pennies to give the customer
    int pennies = calculate_pennies(cents);
    cents = cents - pennies * 1;

    // Sum coins
    int coins = quarters + dimes + nickels + pennies;

    // Print total number of coins to give the customer
    printf("%i\n", coins);
}

int get_cents(void)
{
    int ChangeOwned;
    do
    {
        ChangeOwned = get_int("Change Owned: ");
    }
    while (ChangeOwned < 0);

    return ChangeOwned;
}

int calculate_quarters(int cents)
{
    int quarters = 0, i;

    for (i = 0; cents >= 25; ++i)
    {
        cents = (cents - 25);
        ++ quarters ;
    }
    return quarters;
}

int calculate_dimes(int cents)
{
    int dimes = 0, i;

    for (i = 0; cents >= 10; ++i)
    {
        cents = (cents - 10);
        ++ dimes ;
    }
    return dimes;
}

int calculate_nickels(int cents)
{
    int nickels = 0, i;

    for (i = 0; cents >= 5; ++i)
    {
        cents = (cents - 5);
        ++ nickels ;
    }
    return nickels;
}

int calculate_pennies(int cents)
{
    int pennies = 0, i;

    for (i = 0; cents >= 1; ++i)
    {
        cents = (cents - 1);
        ++ pennies ;
    }
    return pennies;
}
