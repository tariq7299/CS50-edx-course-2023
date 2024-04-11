
#### This is the better/shorter method to solve this practice problem (not all of it is mine ! also not my fav method)
# This method the one with comments as i didnot right any comments in the other method of name "seven-day-average.py"








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
    # Create/Declare a varibale called "previous_cases" which is going to hold the previous day cumulative cases
    previous_cases = dict()
    #  Create a variable called new_cases which is going to hold the new cases of the day
    new_cases = dict()
    #  A for loop which will iterate over each row in the reader Dict
    for row in reader:
        state = row['state']
        cases = int(row['cases'])
        # Create a variable called cases in case if the 'state' (key) we are in was not found or created before, then store the 'cases' (value to that key)
        # , which will become in upcoming steps the previous day cases !
        if state not in previous_cases:
            previous_cases[state] = cases
        # If the 'state' was already created which also means that there is a 'cases' (value to the key 'state'), which was assigned before,
        #  which also means that there is already a previous day cases for the very state
        else:
            # Substract the previous day cases 'previous_cases[state]' form the current day cases 'cases' for that very state we are hovering on rn
            new_case = cases - previous_cases[state]
            # Then the current day cases will become the previous day cases !
            previous_cases[state] = cases

            # The function of the upcoming lines of code is to create the 'new_cases' dict which will hold the new_cases found/appears in each day,
            #  (so it not cumulative like in previous_cases Dict or in the raw data which were provided as a (link) or HTTP get request at the begininng of the code !)

            # If the state is not new_cases dict, then create the (key) which will hold the name of the state,
            #  and its (value) will be an empty list (which later will hold the new_cases appearing each day)
            if state not in new_cases:
                new_cases[state] = []
            # if the  new_cases[state] lists reach 14 elements then remove the first element in order to make room of a new day with its cases,
            #  (and we are keeping it at 14 elements because in the problem the teacher wants us to only collect the most recent 14 days only)
            if len(new_cases[state]) >= 14:
                new_cases[state].pop(0)
            # Finaly add the 'new_case' (the outcome of (cases - previous_cases[state]) which also means the cases which appeared for each day)
            new_cases[state].append(new_case)
    # Finally Finally return the complet 'new_cases' dict, which holds the cases that appeared in each day for the most recent 14 days !
    return new_cases


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):

    # Iterate over the list 'states' which provided by the user (Names of states he typed in terminal)
    for state in states:
        # The sum of all the elements/the new_cases in each state
        sum_of_list = sum(new_cases[state])
        # The sum of the first half of list 'new_cases[state]' (the sum of cases in the first week/previous week)
        First_half_sum = sum(new_cases[state][:len(new_cases[state])//2])
        # The sum of the second half of list 'new_cases[state]' (the sum of cases in the second week/most recent week)
        Second_half_sum = sum_of_list - First_half_sum
        # Average of the new cases in the first week
        First_half_sum_average = First_half_sum / 7
        # Average of the new cases in the most recent week/second week
        Second_half_sum_average = Second_half_sum / 7
        # That upcoming 'try block' to catch ZeroDivisionError if it appears
        try:
            The_percent = round((Second_half_sum_average - First_half_sum_average) / Second_half_sum_average, 2)
        except ZeroDivisionError:
            print("No cases at all in the last week !")
        # Calculate the increase or decrease of cases from prervious week !
        if The_percent > 0:
            print("Seven-Day Averages", "\n", state, "had a 7-day average of", int(Second_half_sum_average), "and an increase of", The_percent, "%" )
        else:
            print("Seven-Day Averages", "\n", state, "had a 7-day average of", int(Second_half_sum_average), "and a decrease of", abs(The_percent), "%" )
main()
