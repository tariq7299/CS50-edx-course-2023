#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
// Made by me
void swap(int, int);
bool check_if_cycle(int loser, int winner);
bool locate_winner (int maybe_winner,int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
         if (strcmp(candidates[i], name) == 0)
         {
            ranks[rank] = i;
            return true;
         }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    int i, j = 0;

    for ( i = 0; i < candidate_count; i++)
    {
        for ( j = i+1 ; j < candidate_count; j++)
        {
        preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}


// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int i, j;

         for ( i = 0; i < candidate_count; i++)
         {
            for ( j = i+1 ; j < candidate_count; j++)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
                else if (preferences[i][j] < preferences[j][i])
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                    pair_count++;
                }
            }
         }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{

// I'm Using here Selection Sort algorithm.
    for (int i = 0; i < pair_count; i++)
    {
        int m = 0;
        int max_votes_idx = i;

        for (int j = i; j < pair_count; j++)
        {
                    /* if "j index" got more votes than "max votes index", the element in "j index" be copied to "max votes index",
                     and that continue to happen in loop until we checks every pair */
                 if (preferences[pairs[j].winner][pairs[j].loser] >
                     preferences[pairs[max_votes_idx].winner][pairs[max_votes_idx].loser])
                {
                    max_votes_idx = j;

            }
        /* When we have the biggets element in "max votes index", we move it its correct place in the descending/decreasing values array,
         by swaping it with i, as i was the orignal/correct place in the decreasing array. ((max_votes_idx was equal to i)) */
        }
        if (max_votes_idx != i)
        {
            swap(i, max_votes_idx);
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // Checks if the loser is the "first winner"/"source of graph" (which will cause a cycle)
        if (!check_if_cycle(pairs[i].loser, pairs[i].winner))
        {
            // if it detects that it causes a cycle it wont make "locked = true", is will skip it, and keep it as false.
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{

    // a loop to check every winner, if he was the source or not, by using the recursive function "locate_winner".
    for (int maybe_winner = 0; maybe_winner < candidate_count; maybe_winner++)
    {
        // A recurcive function.
        if (locate_winner(maybe_winner, 0))
        {
            printf("%s\n", candidates[maybe_winner]);
            return;
        }
    }

/* ((This is another method to implement "print_winner" function,
  although it produces correct results when test it, even with various tests, the check50 won't accept it, and consider it to be incorrect))


    int j = 0, x = 0, _WINNER_ = 0, m = 0;

    for (int p = 0; p < candidate_count; p++)
    {
        for (int o = 0; o < candidate_count; o++ )
            {
                if (locked[p][x])
                {
                    for (j = 0; j < candidate_count; j++ )
                    {
                        x = locked[j][p];
                        m += x;
                    }
                    _WINNER_ = p;
                    if (!m)
                    {
                        printf("%s\n", candidates[_WINNER_]);
                        return;
                    }
                    x = 0;
                    m = 0;
                }
            }

    }
}

*/

}


// swaps two elements in "pairs" array
 void swap(int i, int max_votes_idx)
{
    int temp = pairs[i].winner;
    pairs[i].winner = pairs[max_votes_idx].winner;
    pairs[max_votes_idx].winner = temp;

    temp = pairs[i].loser;
    pairs[i].loser = pairs[max_votes_idx].loser;
    pairs[max_votes_idx].loser = temp;
}

// A recursive function which checks if the any of the winners will cause a cycle in graph
bool check_if_cycle (int loser_End_, int winner_StartOfCycle_)
{
    // (The Base) If loser_End, which was a previous winner ""is actualy the"" source/first winner "winner_StartOfCycle_" (the one with the heighest margin of victory)
    if (loser_End_ == winner_StartOfCycle_)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser_End_][i])
        {
            if (check_if_cycle(i, winner_StartOfCycle_))
            {
                return true;
            }
        }
    }
    return false;
}

// A recursive function which locates the WINNER/WINNERS of election, used in "print_winner" function.
bool locate_winner (int maybe_winner,int loser0)
 {
    if (locked[loser0][maybe_winner])
    {
        return false;
    }
    else
    {
        for (int loser = loser0+1; loser < candidate_count; loser++)
        {
            if (!locate_winner(maybe_winner, loser)) // A recursion function,
            //  which checks if that "maybe_winner" has "true" value of "locked" array with any of the other candidates
            {
                return false;
            }
        }
    }
    return true;
 }
