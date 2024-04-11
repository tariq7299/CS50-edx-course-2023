// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>



#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
// A variable which is used to count the size of our hash table elements/values/kes/words
int count = 0;

// TODO: Choose number of buckets in hash table
// I made N to be equals to 2^p
const unsigned int N = 512;

// Hash table
// Here we created it in "stack" memory however later we alocated for each bucket/index a place in memory so to be
 // stored in the "Heab", (So also because of this move, there is no need to initialize every bucket/index with a value of "NULL")
node *table[N];


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int idx = hash(word);
    // A temperory ponter of tupe "node", to cursor the Linked List found at that specific lcation of table[idx]
    node *cursor = table[idx];

//To iretate over the whole linked list found at that specefic location of table[idx] (Bucket)
    while (cursor != NULL)
    {
// Compares "word" (case-insensitively)at a specific location in the hash table (cursor->word) TO the "word" which has been enterd
        if(strcasecmp(cursor->word, word) == 0)
        {
            // The Word is found !
            return true;
        }
        else
        {
            // Go to next location in the Linked list
            cursor = cursor->next;
        }
    }
    // The word is not found !
    return false;
}

// Hashes word to a number
// This is a hashing function
unsigned int hash(const char *word)
{
    // "i" is a counter used in the "for" loops,, "n" used to store the number of char found in "word"
    // "key" is the a variable used in the hashing process,, "A_A" is a variable used in the hashing which is one of the ingredients required in hashing via "Multiplaction Method",
    // It should be number less than 1 and more the 0, and i have learned that it is best to be equal to the fraction part of "Golden Ratio" number (1.618033988749894), so the fraction part will be equal to (0.618033988749894)
    // "intpart" is a variable which is a necessary ingerdiant hashing via "Multiplaction Method", which is used to store the "Intger" part of output of "modf()"
    int i, n;
    int key = 0;
    double A_A = 0.618, intpart = 0;

    // TODO: Improve this hash function
    for (i = 0, n = strlen(word); i < n; i++)
    {
        // If we use this instead, check() will no longer ignore if the user typed capital or small characters in the terminal
        // to be looked up for (it will be case sensitive !, and dont want that), we want to able to lookup/check words regardless of whether the characters of the words is capital or small !
        // key += word[i];
        key += tolower(word[i]);
    }
    //
    int idx = floor(N * modf(key * A_A, &intpart));
    return idx;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    char word_tmp[LENGTH + 1];
    FILE *Dictionary_file = fopen(dictionary, "r");
    if(Dictionary_file == NULL)
    {
        printf("Error opening file!\n");
        fclose(Dictionary_file);
        return false;
    }
    // Copies Every single word sperately found in dictionary then pastes it to word_tmp
    while(fscanf(Dictionary_file, "%s", word_tmp) != EOF)
    {
        // This Temperaroy pointer/node created to alocate memory and store each word from in dictionary to our hash table[] of size N, and it will be freed later in "unload()"
        node *n = malloc(sizeof(node));
        n->next = NULL;

        if(n == NULL)
        {
            printf("NOT ENOUGH MEMORY !");
            return false;
        }
        strcpy(n->word, word_tmp);
        // Here we output the Hash value/idx/Bucket number......etc(what ever you want to name it)
        int idx = hash(n->word);

        // scenario 0: table[idx] is Empty
        if(table[idx] == NULL)
        {
            // scenario 0: That the Buckt is empty
            // Insert Directly
            table[idx] = n;
            n->next = NULL;
            count ++;

        }
        else
        {
            bool k = false;
            node *cursor = table[idx];
            while (cursor != NULL)
            {
                // scenario 1: The table[idx] is occupied, However it is occupied with the same "KEY"/word, so Update the Value (which in reality doese nothing !)
                if(strcmp(cursor->word, n->word) == 0)
                {
                    strcpy(cursor->word, n->word);

                    // No need to type "count++", as there is no new word were added !

                    // This to tell whether this part happend or not
                    k = true;
                    break;
                }
                else
                {
                    cursor = cursor->next;
                }
            }
            // So if k was true, which will mean the key just enterd is already excists and there is no need add this "key"/(word) to the Linked list found in that very bucket
            if (!k)
            {
            // scenario 2: The table[idx] is occupied, However it is occupied with a different/new "KEY"/word, so link the new node (as a stacking method and not queuing methed)
            //to the Linked-List found in that speceifc table[idx]
            n->next = table[idx];
            table[idx] = n;
            count ++;
            }
        }
    }
    // A function to see the entries/words of my hash table, (Not ver practical as the words/values are too much !, which will overwhelm the termilnal !)
    // // void print_table()
    // // {
    //     node *cursor = malloc(sizeof(node));
    //     cursor->next = NULL;
    //         printf("***START***\n");
    //         for(int i = 0; i < N; i++)
    //         {
    //             if(table[i] == NULL)
    //             {
    //                 printf("\t%i\t---\n", i);
    //             }
    //             else
    //             {
    //                 printf("\t%i\t", i);
    //                 cursor = table[i];
    //                 while (cursor != NULL)
    //                 {
    //                     printf("%s\t", cursor->word);
    //                     cursor = cursor->next;

    //                 }
    //                 printf("\n");
    //             }
    //         }
    //         printf("***END***\n");
    //         printf("SIZE: %i\n", size());
    // // }

        fclose(Dictionary_file);

    // scenario 3: The end of dictionary file/No more words !
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
 unsigned int size(void)
{
    // TODO
    // It counted in load(), with every new word intered in the tabel[], so here the size() just "return" the GLOBAL variabl "count"
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
    node *cursor = table[i];
    while(cursor != NULL)
    {
        node *next = cursor->next;
        free(cursor);
        cursor = next;
    }
    }
    return true;

}
