# TODO

def main():
    # This block of code will keep prompting the user untill he inputs a valid answer, (the valid answer is any number more bigger than 0)
    while True:
        try:
            credit_no = input('Number: ')
            if int(credit_no) > 0:
                break
        except ValueError:
            pass
    # Initilizing this variable which is used store the number of digits og the credit card
    number_of_digits = 0
    # It iterate over the credit_no (str) characters
    for i in credit_no:
        number_of_digits +=1

    # necessary variables initionlizing which are used in Luhn's algorithm
    sum1 = sum2 = sum3 = 0

    # The following block of code will implement the Luhn's algorithm
     # Important Note:
    # - American Express uses 15-digit numbers, MasterCard uses 16-digit numbers, and Visa uses 13- and 16-digit numbers.
     #- All American Express numbers start with 34 or 37; most MasterCard numbers start with 51, 52, 53, 54, or 55,
     #(they also have some other potential starting numbers which we won’t concern ourselves with for this problem); and all Visa numbers start with 4.
     #- Luhns algorithm should equal (sum1+sum2=sum3) some numeber from 10 to 90.


    # This will multibly each other number from the end of credit card (starting from the right) by 2,
    #  (also notice that we will start from the second digit starting from the right)
    for i in range(len(credit_no)-2, -1, -2):
        # "maybe_a_two_digits_no" will store the result of each operation,
        # and will be used later to check if it stores a two digit number or just one digit
        maybe_a_two_digits_no = int(credit_no[i]) * 2
        # If "mayba_a_two_digits_no" bigger than 9 than means it store a two digits number,
        #  so after the if i will divide that number and add the digits indiviually/seperatly to the variable "sum1"
        if maybe_a_two_digits_no > 9:
            # 'int()' to just take the integer part of 'maybe_a_two_digits_no / 10'
            sum1 += int(maybe_a_two_digits_no / 10)
            sum1 += maybe_a_two_digits_no % 10
        # And if it isn't bigger than "9" so that means maybe_a_two_digits_no stores a one digit number, so add it directly to "sum1"
        else:
            sum1 += maybe_a_two_digits_no
    # This will add each other number starting from the end of the credit card (right to left) to each other, (also we here are starting from the very first digit from right to left)
    for i in range(len(credit_no)-1, -1, -2):
        # Add store the total in a variable called "sum2"
        sum2 += int(credit_no[i])

    # Finaly add the previous to sums at each other and store the result in a new variable called "sum3"
    sum3 = sum1 + sum2

    # Calculated  the second/last digit of the number (first digit of the number from the right) and then stores it in a variable called "last_digit"
    last_digit = sum3 % 10


    # If the 'last_digit' equals to '0' that means that could be a ligit credit card number, which passed the Luhn's verification teqnique (algorithm)
    if last_digit == 0:

      # The upcoming block of code Tests for which company the credit card belongs to.

      
        if number_of_digits == 15:
            if (credit_no[0] == '3' and credit_no[1] == '7') or (credit_no[0] == '3' and credit_no[1] == '7'):
                print('AMEX')
            else:
                print('INVALID')
        elif number_of_digits == 16 or number_of_digits == 13:
            if (credit_no[0] == '5' and credit_no[1] == '1') or (credit_no[0] == '5' and credit_no[1] == '2') or (credit_no[0] == '5' and credit_no[1] == '3') or (credit_no[0] == '5' and credit_no[14] == '4') or (credit_no[0] == '5' and credit_no[1] == '5'):
                print('MASTERCARD')
            elif credit_no[0] == '4' or credit_no[0] == '4':
                    print('VISA')
            else:
                    print('INVALID')
        else:
           print('INVALID')
    else:
           print('INVALID')



main()