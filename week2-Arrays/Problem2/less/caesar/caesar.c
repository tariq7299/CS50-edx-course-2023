#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool check_if_not_number(string);
const string upper_AI = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const string lower_AI ={"abcdefghijklmnopqrstuvwxyz"};
string Cipher_upper_case_letter(string, int, int);
string Cipher_lower_case_letter(string, int, int);
void prints_cipher_text(string);

// Counters in (for loops) variables inside functions.
int i, m, c;

int main(int argc, string argv[])
{

// Checks if user didnot enter any numbers after ./caesar.
    while (argc < 2)
 {
    printf("Usage: ./caesar key\n");
    return 1;
 }

    int k = 0;
    string u = argv[1];

// atoi() function used to transform k (string) into k (int).
// k is the cipher key which the user typed in.
    k = atoi(argv[1]);

// Check whether the user have enterd k "key" as anything other than a number or entered more than one argument.
   while (check_if_not_number(u) || argc != 2)
 {
    printf("Usage: ./caesar key\n");
    return 1;
 }

// "T" = Text which the user type in.
    string T = get_string("plaintext: ");

// Number of Characters in "T".
    int number_of_char = strlen(T);
    Cipher_upper_case_letter(T, number_of_char, k);
    Cipher_lower_case_letter(T, number_of_char, k);
    prints_cipher_text(T);

    return 0;
}

/* Ciphers every "upper Case letters" in T then replaces the old normal unciphered letters with the new ciphered(incoded) letters using k "key"
 also leaves any character other than letters as it is. */
string Cipher_upper_case_letter(string T, int number_of_char, int k)
{
    c = 0;
         for (i = 0; i < number_of_char; ++i)
        {
         if (isupper(T[i]))
         {
            int upper_letter = T[i];
            for(m = 0; m < 26; ++m)
            {
                if (upper_AI[m] == upper_letter )
                {
                    c = (m + k) % 26;
                }
            }
              T[i] = upper_AI[c];
          }
        }
        return T;
}

/* Ciphers every "lower Case letters" in T then replaces the old normal unciphered letters with the new ciphered(incoded) letters using k "key"
 also leaves any character other than letters as it is. */
string Cipher_lower_case_letter(string T, int number_of_char, int k)
{
    c = 0;
      for (i = 0; i < number_of_char; ++i)
        {
        if (islower(T[i]))
          {
            int lower_letter = T[i];
            for(m = 0; m < 26; ++m)
            {
                if (lower_AI[m] == lower_letter)
                {
                    c = (m + k) % 26;
               }
            }
             T[i] = lower_AI[c];
          }
        }
        return T;
}


// Just prints New Encoded(ciphered) text "T".
void prints_cipher_text(string T)
{
        printf("ciphertext: %s\n", T);
}

// Checks whether the key is a number or not.
bool check_if_not_number(string u)
{
    int n = strlen(u), j = 0;

   for (i = 0; i < n; ++i)
   {
    j = isdigit(u[i]);
    if (j == 0)
    {
    return true;
    }
   }
    return false;
}







