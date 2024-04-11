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

    # Calculate the number of coins required for each denomination
    quarters = cents // 25
    dimes = (cents % 25) // 10
    nickels = ((cents % 25) % 10) // 5
    pennies = ((cents % 25) % 10) % 5

    # Print the total number of coins required
    print(quarters + dimes + nickels + pennies)

if __name__ == "__main__":
    main()
