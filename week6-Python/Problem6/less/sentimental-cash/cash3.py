def main():
    # Ask user for change owed in dollars
    while True:
        dollars = input("Change owed: ")
        try:
            dollars = float(dollars)
            if dollars >= 0:
                break
        except ValueError:
            pass

    # Convert dollars to cents
    cents = round(dollars * 100)

    # Define a list of denominations
    denominations = [25, 10, 5, 1]

    # Calculate the number of coins required for each denomination
    total_coins = 0
    for denomination in denominations:
        num_coins = cents // denomination
        total_coins += num_coins
        cents -= num_coins * denomination

    # Print the total number of coins required
    print(total_coins)

if __name__ == "__main__":
    main()