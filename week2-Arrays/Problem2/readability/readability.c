#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


int count_Words(string);
int count_Letters(int);
int count_Scentences(string);
int count_Spaces(string);
int count_end_of_sentences_Char(string);
int count_Sentences(int);
int Calculate_Grade_Level(int, int, int);
int count_char_excluding_Letters(string);


int p, i, m, number_of_Spaces, number_of_char;
double index;
const string space = " ";
// End of sentences characters array(characters which located at the end of sentences)
char end_of_sentences_Char[] = {"!.?"};


int main(void)
{
    int number_of_end_of_sentences_Char, number_of_Letters, number_of_Sentences, number_of_Words, number_of_char_excluding_letters;
    string text = get_string("Text: ");
    number_of_char = strlen(text);
    number_of_Spaces = count_Spaces(text);
    number_of_char_excluding_letters = count_char_excluding_Letters(text);
    number_of_end_of_sentences_Char = count_end_of_sentences_Char(text);
    number_of_Letters = count_Letters(number_of_char_excluding_letters);
    number_of_Words = count_Words(text);
    number_of_Sentences = count_Sentences(number_of_end_of_sentences_Char);
    index = Calculate_Grade_Level(number_of_Letters, number_of_Sentences, number_of_Words);
}

// Calculates the number of spaces the text contains
int count_Spaces(string text)
{
    p = 0;
    for (i = 0; i < number_of_char; i++)
    {
        if (text[i] == space[0])
        {
            p++;
        }
    }
    return p;
}

// Calculates the number of Non-Alphapitacl characters the text contains
int count_char_excluding_Letters(string text)
{
    p = 0;
    for (i = 0; i < number_of_char; i++)
    {
        if (isalpha(text[i]) == 0)
        {
            p++;
        }
    }
    return p;
}

// Calculates the number of end of sentences characters found in text
int count_end_of_sentences_Char(string text)
{
    p = 0;
    for (i = 0; i < number_of_char; i++)
    {
        int character = text[i];
        for (m = 0; m < 5; m++)
        {
            if (character == end_of_sentences_Char[m])
            {
                p++;
            }
        }
    }
    return p;
}

// Calculates the number of letters characters text contains
int count_Letters(int number_of_char_excluding_letters)
{
    int number_of_Letters = 0;
    number_of_Letters = number_of_char - number_of_char_excluding_letters;
    return number_of_Letters;
}


// Calculates the number of words characters text contains
int count_Words(string text)
{
    int number_of_Words = 0;
    number_of_Words  = number_of_Spaces + 1;
    return number_of_Words;
}

// Calculates the number of sentences characters text contains
int count_Sentences(number_of_end_of_sentences_Char)
{
    int number_of_Sentences = number_of_end_of_sentences_Char;
    return number_of_Sentences;
}

// Calculates grade level of text by using The Coleman-Liau formula ((index = 0.0588 * L - 0.296 * S - 15.8))
int Calculate_Grade_Level(number_of_Letters, number_of_Sentences, number_of_Words)
{
    double S = (double) number_of_Sentences / (double) number_of_Words * 100;
    double L = (double) number_of_Letters / (double) number_of_Words * 100;
    index = 0.0588 * L - 0.296 * S - 15.8;

    // Decides whether the text is before Grade 1, after grade 16, or between them.
    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", (int) round(index));
    }
    return index;
}
