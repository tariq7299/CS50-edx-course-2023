#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
string upper_alpha_array = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
int x, n, m, upper_alpha, sum;

int compute_score(string);
void compare_scores_Then_print_winner(int, int);

int main(void)
{
    // Get input words from both players
   string word1 = get_string("Player 1: ");
   string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // prints the winner
compare_scores_Then_print_winner(score1, score2);
}

// Function which calculate score for each player
int compute_score(string word)
{
  upper_alpha = 0, sum = 0;

    for(x = 0, n = strlen(word); x < n; x++)
    {
      // Transform every LowerCase letter to UpperCase
        upper_alpha = toupper(word[x]);

        // Finds every element value in POINTS[] Array
        for(m = 0; m < 26; m++)
        {
          if (upper_alpha == upper_alpha_array[m])
          {
            sum += POINTS[m];
          }
        }
        }
        return sum;
}

// Function which compares player 1's  score with player 2's socre then prints the winner
void compare_scores_Then_print_winner(score1, score2)
{
  if(score1 == score2)
  {
   printf("Tie!\n");
  }
  else if (score1 > score2)
  {
   printf("Player 1 wins !\n");
  }
  else
  {
    printf("Player 2 wins !\n");
  }
}
