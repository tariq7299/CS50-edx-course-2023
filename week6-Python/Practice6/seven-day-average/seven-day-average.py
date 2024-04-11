#### This the method created by me, which also m fav method, unforunatey there no comments ! HaVe FUN !!!




import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)
    print(new_cases)
    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)

# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    new_cases = dict()
    previous_cases = dict()
    element_count = 0
    days_count = 0
    reader_as_a_list = list(reader)  # Convert reader object to list

    while True:
        idx = len(reader_as_a_list) - 1 - element_count
        row = reader_as_a_list[idx]
        current_date = row['date']
        for idx2 in range(len(reader_as_a_list)- 1, -1, -1):
            row2 = reader_as_a_list[idx2]
            if row2['date'] == current_date:
                element_count += 1
        days_count += 1
        if days_count == 15:
                break
        # print("days : ", days_count,"-----","Index number :", len(reader_as_a_list) - element_count)

    # Creating previous_cases Dict, which stores the most recent 15 days
    for idx in range(len(reader_as_a_list)-1, len(reader_as_a_list) - element_count, -1):
        row = reader_as_a_list[idx]
        state =  row['state']
        cases = row['cases']
        if state not in previous_cases:
            previous_cases[state] = []
        previous_cases[state].append(cases)

        # Counter i, which is used to go through all of the elements of values list[] of the keys of previous_cases Dict
    i = 0

    for state in previous_cases:
            while i < len(previous_cases[state]) -1:
                    today_cases = int(previous_cases[state][i]) - int(previous_cases[state][i+1])
                    i += 1
                    if state not in new_cases:
                        new_cases[state] = []
                    new_cases[state].append(today_cases)
                    if len(new_cases[state]) > 14:
                        new_cases[state].pop(14)
            # Reset the counter
            i = 0

    return new_cases

       ## # If you want to make new_cases contain the cases which apperes as cumulative cases and not todays_new cases only (not previous_cases - current_day_cases)

    # while True:
    #     idx = len(reader_as_a_list) - 1 - element_count  # Use indexing on list
    #     row = reader_as_a_list[idx]
    #     current_date = row['date']
    #     for idx2 in range(len(reader_as_a_list)- 1, -1, -1):
    #         row2 = reader_as_a_list[idx2]
    #         if row2['date'] == current_date:
    #             element_count += 1
    #     days_count += 1
    #     if days_count == 14:
    #             break
    # print("days : ", days_count,"-----","Index number :", len(reader_as_a_list) - element_count)

    # for idx in range(len(reader_as_a_list)-1, len(reader_as_a_list) - element_count, -1):
    #     row = reader_as_a_list[idx]
    #     state =  row['state']
    #     cases = row['cases']
    #     if state not in new_cases:
    #          new_cases[state] = []
    #     new_cases[state].append(cases)
    # print("new_cases dictionary : ", new_cases)

# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        sum_of_list = sum(new_cases[state])
        First_half_sum = sum(new_cases[state][:len(new_cases[state])//2])
        Second_half_sum = sum_of_list - First_half_sum
        First_half_sum_average = First_half_sum / 7
        Second_half_sum_average = Second_half_sum / 7
        try:
            The_percent = round((First_half_sum_average - Second_half_sum_average) / First_half_sum_average, 2)
        except ZeroDivisionError:
            print("No cases at all in the last week !")
        if The_percent > 0:
            print("Seven-Day Averages", "\n", state, "had a 7-day average of", int(First_half_sum_average), "and an increase of", The_percent, "%" )
        else:
            print("Seven-Day Averages", "\n", state, "had a 7-day average of", int(First_half_sum_average), "and a decrease of", abs(The_percent), "%" )








main()
