#include <cs50.h>
#include <stdio.h>


int main(void)
{
   long Credit_Number = 0;
   // Prompts the user to enter a valid credit card number (zero is ok)
   do {
         Credit_Number = get_long("Credit/Debit Number \n");

   } while (Credit_Number < 0);

    const long Credit_NumberB = Credit_Number;
    int Number_of_Credit_Digits = 0, A_Digits;
    int i = 0;

    // Gets the total Number_of_Credit_Card_Digits
    for (i = 0; Credit_Number > 0 ; ++i)
    {
        A_Digits = Credit_Nwumber % 10;
        Credit_Number = Credit_Number - A_Digits;
        Credit_Number = Credit_Number / 10;
        ++ Number_of_Credit_Digits;
    }

    // Assigns the "total Number_of_Credit_Card_Digits" to "credit_Digits" array
    int credit_Digits[Number_of_Credit_Digits];

    // Assigns the Constant "Credit_NumberB" to "Credit_Number" to be able to use "Credit_Number" again
    Credit_Number = Credit_NumberB;

    // Assigns the Each digit of the credit card to "credit_Digits" array
    for (i = 0; i < Number_of_Credit_Digits; ++i)
    {
        credit_Digits[i] = Credit_Number % 10;
        Credit_Number = Credit_Number - credit_Digits[i];
        Credit_Number = Credit_Number / 10;
    }

    // Calculates "Luhn's algorithm"
    int sum1 = 0, sum2 = 0, sum3 = 0, Maybe_Two_Digits_no = 0;
    for (i = 1; i < Number_of_Credit_Digits; i += 2)
    {
        Maybe_Two_Digits_no = credit_Digits[i] * 2;
        // Sepertates any two Digits number and outputs every digit in the number separately, in order to get sum1 of card card's (digits*2).
        if (Maybe_Two_Digits_no > 9)
        {
            sum1 += Maybe_Two_Digits_no / 10;
            sum1 += Maybe_Two_Digits_no % 10;
        }
        else
        {
            sum1 += Maybe_Two_Digits_no;
        }
    }

    //gets sum2 of credit card card's digits.
    for (i = 0; i < Number_of_Credit_Digits; i += 2)
    {
        sum2 += credit_Digits[i];
    }
    /* Important Note:
    - American Express uses 15-digit numbers, MasterCard uses 16-digit numbers, and Visa uses 13- and 16-digit numbers.
    - All American Express numbers start with 34 or 37; most MasterCard numbers start with 51, 52, 53, 54, or 55,
    (they also have some other potential starting numbers which we won’t concern ourselves with for this problem); and all Visa numbers start with 4.
    - Luhns algorithm should equal (sum1+sum2=sum3) some numeber from 10 to 90.
    */
    //Tests if the digit on right is equal to 0.
    int Last_Digit = 0;
    sum3 = sum1 + sum2;
    Last_Digit = sum3 % 10 ;
    if (Last_Digit == 0)
    {
      // All the upcoming Tests for which company the credit card belongs to.
       if (Number_of_Credit_Digits == 15)
       {
            if ((credit_Digits[14] == 3 && credit_Digits[13] == 7) || (credit_Digits[14] == 3 && credit_Digits[13] == 7))
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
      }
      else if (Number_of_Credit_Digits == 16 || Number_of_Credit_Digits == 13)
      {
           if ((credit_Digits[15] == 5 && credit_Digits[14] == 1) || (credit_Digits[15] == 5 && credit_Digits[14] == 2)
               || (credit_Digits[15] == 5 && credit_Digits[14] == 3)  || (credit_Digits[15] == 5 && credit_Digits[14] == 4)
               || (credit_Digits[15] == 5 && credit_Digits[14] == 5))
           {
                printf("MASTERCARD\n");
           }
           else if (credit_Digits[15] == 4 || credit_Digits[12] == 4)
           {
                    printf("VISA\n");
           }
           else
           {
                    printf("INVALID\n");
           }
      }
      else
      {
           printf("INVALID\n");
      }
   }
   else
   {
           printf("INVALID\n");
   }
}

                                                                                            /* Allah Akbar wa lelah elhamd */
