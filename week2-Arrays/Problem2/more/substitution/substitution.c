#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool check_if_number(string);
bool check_if_not_letter(string);
bool check_if_dublicated_letter(string, string);
const string upper_AI = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const string lower_AI ={"abcdefghijklmnopqrstuvwxyz"};
string Cipher_upper_case_letter(string, int, string, string);
string Cipher_lower_case_letter(string, int, string, string);
void prints_cipher_text(string);


// Counters in (for loops) variables inside functions.
int i, m, l;

int main(int argc, string argv[])
{



// Checks whether the user hasn't privided any argument
  while ((argc < 2))
  {
         printf("Usage: ./substitution key\n");
         return 1;
  }


  string k = argv[1];
  string kL = argv[1];
  string kU = argv[1];
  string k_if_dup = argv[1];
  int number_of_char_in_key = strlen(k);

  /* Checks if any digit where to be found in key ||or||Check whether the user have enterd k "key" as anything other than a alphabetacl ||or|| if the user
  entered more than one argument ||or|| if key were containing less than 26 characters of alphabit ||or|| if there were any dublicated characters x. */
  while (check_if_number(k) || check_if_not_letter(k) || argc != 2 || number_of_char_in_key != 26
  || check_if_dublicated_letter(k_if_dup, k))
  {
   printf("Usage: ./substitution key\n");
   return 1;
   }


// "T" = Text which the user type in.
    string T = get_string("plaintext: ");

   // Number of Characters in "T".
   int number_of_char_in_text = strlen(T);

   Cipher_upper_case_letter(T, number_of_char_in_text, kU, k);
   Cipher_lower_case_letter(T, number_of_char_in_text, kL, k);
   prints_cipher_text(T);

    return 0;
}

/* Ciphers every "upper Case letters" in T then replaces the old normal unciphered letters with the new ciphered(incoded) letters using k "key"
 also leaves any character other than letters as it is. */
string Cipher_upper_case_letter(string T, int number_of_char_in_text, string kU, string k)
{

      for (i = 0; i < number_of_char_in_text; ++i)
      {
          if (isupper(T[i]))
          {
            int unciphered_upper_letter = T[i];

            for (m = 0; m < 26; ++m)
            {

                if (upper_AI[m] == unciphered_upper_letter)
                {
                    T[i] = toupper(kU[m]);



                }

           }
        }
      }
  return T;
}

// /* Ciphers every "lower Case letters" in T then replaces the old normal unciphered letters with the new ciphered(incoded) letters using k "key"
//  also leaves any character other than letters as it is. */
string Cipher_lower_case_letter(string T, int number_of_char_in_text, string kL, string k)
{
           for (i = 0; i < number_of_char_in_text; ++i)
         {
          if (islower(T[i]))
          {
            int unciphered_lower_letter = T[i];

            for(m = 0; m < 26; ++m)
            {
                if (lower_AI[m] == unciphered_lower_letter)
                {
                    T[i] = tolower(kL[m]);
                }
            }
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
bool check_if_number(string k)
{

   int n = strlen(k), j = 0;

   for (i = 0; i < n; ++i)
   {
        j = isdigit(k[i]);
        if (j == 0)
        {
        return false;
        }
   }
   return true;
}

// Checks whether the user has typed in anything other than a alphabit.
bool check_if_not_letter(string k)
{
   int n = strlen(k), j = 0;

   for (i = 0; i < n; ++i)
   {
        j = isalpha(k[i]);
        if (j == 0)
        {
        return true;
        }
   }
  return false;
}


// Checks whether there was any duplicated characters.
bool check_if_dublicated_letter(string k_if_dup, string k)
{
  int x, sum = 0;
  for (m = 0; m < 26; ++m)
  {
       k_if_dup[m] = toupper(k_if_dup[m]);
       x = k_if_dup[m];
       sum += x;
  }
  if (sum != 2015)
  {
      return true;
  }
  else
  {
  return false;
  }

}






