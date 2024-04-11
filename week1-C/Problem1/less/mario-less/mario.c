#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;

    // Prompt the user for the height of the half-pyramid
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    // Print out the half-pyramid
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (i + j < height - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }
        printf("\n");
    }

    return 0;
}