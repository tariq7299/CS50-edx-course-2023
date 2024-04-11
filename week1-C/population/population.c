

// Here, I am using some advanced techniques that we haven't yet learned because I am solving this particular problem after I have already finished the course. Back then, I didn't answer this part when I started.




#include <stdio.h>


int main(void) {
    int starting_population;
    int ending_population;
    int ending_population_tmp = 0;
    int years = 0;

    // Prompt the user to enter the strting populaation until a valid input is received.
    do {
        printf("Enter the starting number of population: ");

        // Attempt to read an integer from user input.
        if (scanf("%d", &starting_population) != 1) {
            // If scanf fails, clear the input buffer and display an error message.
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a valid number.\n");
        }
    } while (starting_population < 9); // Repeat ntil a valid starting population is entered.

    // Prompt the useerr to enter the ending population until valid input is received.
    do {
        printf("Enter the ending number of population: ");

        // Attempt to read an integer from user input.
        if (scanf("%d", &ending_population) != 1) {
            // If scanf fails, clear the input buffer and display an error message.
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a valid number.\n");
        } else if (ending_population < starting_population) {
            // Check if ending population is less than starting population.
            printf("Ending population must be greater than starting population.\n");
        }
        // Repeat until valid ennding population is entered.
    } while (ending_population < starting_population);

    if (starting_population == ending_population) {
        // If starting and ending populations are the same, print years (0) and exit.
        printf("Years: %d\n", years);
        return 0;
    }

    // Calculate the number of years required to reach the ending population.
    while (ending_population_tmp < ending_population) {
        ending_population_tmp = (starting_population / 3 - starting_population / 4) + starting_population;

        // Update starting population and increment the years counter.
        starting_population = ending_population_tmp;
        years++;
    }

    // Print the calcuclated number of years.
    printf("Years: %d\n", years);
    return 0;
}
