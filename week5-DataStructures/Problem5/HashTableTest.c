#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NAME 256

typedef struct node
{
    char word[MAX_NAME];
    struct node *next;
}node;

const unsigned int N = 16;

// Hash table
node *table[N];

int count = 0;

bool hash_table_insert(node *);
unsigned int hash(char *);
void print_table();
bool check(char *);


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./test0 'word'\n");
        return 1;
    }
    node f = {.word="apple"};
    node a = {.word = "app"};
    node b = {.word = "fool"};
    node c = {.word = "kalb"};
    node g = {.word = "solla"};
    node h = {.word = "bolla"};
    node k = {.word = "holla"};
    node H = {.word = "kolla"};
    node l = {.word = "Nolla"};
    node e = {.word = "NfsLa@"};
    node T = {.word = "NdsLa"};
    node TH = {.word = "dsa.."};
    node THR = {.word = "daa,."};
    node THRT = {.word = "trzf."};
    node THRTD = {.word = "432c"};
    node THRTDG = {.word = "..21k"};
    node THRTDG3 = {.word = "NO@dLLa"};
    node kc = {.word = "holla"};
    node THRTDGh = {.word = "a[/.3,]"};
    // printf("idx: %i\n", idx);
    // printf("intpart: %f\n", intpart);
    // printf("N: %i\n", N);
    // printf("A_A: %f\n", A_A);
    // printf("key: %i\n", key);
    // printf("n: %i\n", n);

    for (int i = 0; i < N; i++)
        {
            table[i] = NULL;
        }

    hash_table_insert(&f);
    hash_table_insert(&a);
    hash_table_insert(&b);
    hash_table_insert(&c);
    hash_table_insert(&g);
    hash_table_insert(&h);
    hash_table_insert(&k);
    hash_table_insert(&H);
    hash_table_insert(&l);
    hash_table_insert(&e);
    hash_table_insert(&T);
    hash_table_insert(&TH);
    hash_table_insert(&THR);
    hash_table_insert(&THRT);
    hash_table_insert(&THRTD);
    hash_table_insert(&THRTDG);
    hash_table_insert(&THRTDG3);
    hash_table_insert(&THRTDGh);
    hash_table_insert(&e);
    hash_table_insert(&kc);

    print_table();

    if(check(argv[1]))
    {
        printf("Found !\n");
    }
    else
    {
        printf("The word is not found !\n");
    }
    return 0;




}


unsigned int hash(char *word)
{
        int i, n;
        int key = 0;
        double A_A = 0.618, intpart = 0, inttest_part;

        // TODO: Improve this hash function
        for (i = 0, n = strlen(word); i < n; i++)
        {
            // If we use this instead, check() will no longer ignore if the user typed capital or small characters in the terminal
            // to be looked up for (it will be case sensitive !, and dont want that), we want to able to lookup/check words regardless of whether the characters of the words is capital or small !
            // key += word[i];
            key += tolower(word[i]);
        }
        int idx = floor(N * modf(key * A_A, &intpart));
        return idx;
}

bool hash_table_insert(node *n)
{

        int idx = hash(n->word);

        // scenario 0: table[idx] is Empty
        if(table[idx] == NULL)
        {
            // Insert Directly
            table[idx] = n;
            n->next = NULL;
            count ++;
            return true;
        }
        else
        {
            node *cursor = table[idx];
            while (cursor != NULL)
            {
                // scenario 1: The table[idx] is occupied, However it is occupied with the same "KEY", so Update the Value
                if(strcmp(cursor->word, n->word) == 0)
                {
                    strcpy(cursor->word, n->word);
                    return true;
                }
                else
                {
                    cursor = cursor->next;
                }
            }

            // scenario 2: The table[idx] is occupied, However it is occupied with a different "KEY", so link the new node (as a stack)
            //to the Linked-List found in that speceifc table[idx]
            n->next = table[idx];
            table[idx] = n;
            count ++;
            return true;

        }
}




unsigned int size(void)
{
    // TODO
    return count;
}


bool check(char *word)
{

        int idx = hash(word);
        node *cursor = table[idx];

        while (cursor != NULL)
    {

        if(strcasecmp(cursor->word, word) == 0)
        {
            // The Word is found !
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
        // The word is not found !
        return false;



    }



void print_table()
    {
        node *cursor = malloc(sizeof(node));
        cursor->next = NULL;
            printf("***START***\n");
            for(int i = 0; i < N; i++)
            {
                if(table[i] == NULL)
                {
                    printf("\t%i\t---\n", i);
                }
                else
                {
                    printf("\t%i\t", i);
                    cursor = table[i];
                    while (cursor != NULL)
                    {
                        printf("%s\t", cursor->word);
                        cursor = cursor->next;

                    }
                    printf("\n");
                }
            }
            printf("***END***\n");
            printf("SIZE: %i\n", size());
    }







