import cs50




def main():

    # Here it prompts the user for the cash owned
    cents = get_cents()

    # Calculates the number of quarters in the cash owned
    quarters = calculate_quarters(cents)
    # Then it subsctract all quarters from the cash owned then now "cents" is the cash owned left without containing any quarter
    cents = cents - quarters * 25

    # Calculates the number of dimes in the cash owned
    dimes = calculate_dimes(cents)
    # Then it subsctract all dimes from the cash owned then now "cents" is the cash owned left without containing any dimes
    cents = cents - dimes * 10

    # Calculates the number of nickels in the cash owned
    nickels = calculate_nickels(cents)
    # Then it subsctract all nickels from the cash owned then now "cents" is the cash owned left without containing any nickels
    cents = cents - nickels * 5

    # Calculates the number of pennies in the cash owned
    pennies = calculate_pennies(cents)
    # Then it subsctract all pennies from the cash owned then now "cents" is the cash owned left without containing any pennies
    cents = cents - pennies * 1

    # Finally it adds all the coins types to each other, then prints the coins cowned !
    coins = quarters + dimes + nickels + pennies
    print(f'{int(coins)}')






def get_cents():

    # This will keep prompting untill the user gives valid answer
    while True:
        dollars = cs50.get_float('change owned: ')
        if dollars >= 0:
            break
    # Converts the change/cash owned typed by user from dollars to cents
    cents = round(dollars * 100)
    return cents

def calculate_quarters(cents):
    remainder = cents % 25
    quarters = (cents - remainder) / 25
    return quarters

def calculate_dimes(cents):
    remainder = cents % 10
    dimes = (cents - remainder) / 10
    return dimes

def calculate_nickels(cents):
    remainder = cents % 5
    nickels = (cents - remainder) / 5
    return nickels

def calculate_pennies(cents):
    remainder = cents % 1
    pennies = (cents - remainder) / 1
    return pennies









main()





