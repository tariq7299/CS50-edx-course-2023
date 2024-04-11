import csv
import sys


def main():

    # TODO: Check for command-line usage

        # If there waeren't 3 command line arguments, then The program is going to exit with a message
    if len(sys.argv) != 3:
        sys.exit('Usage: dna.py database file name sequence file name')
    else:
        # However if the user provided 3 command line arguments then the program would continue

        # Create an empty list 'longest_run_list' which will be used later to store the longest runs of every subsequence(STR) of every sequence
        longest_run_list = []

        # Open two files, the database files which contains some people with each STR and its longest run in the person's DNA,
        # And the other file contain a sequence of a DNA of some person (Which may belong to someone in the database)
        with open(sys.argv[1], 'r') as f1, open(sys.argv[2], 'r') as f2:

    # TODO: Read database file into a variable

            database_reader = csv.DictReader(f1)

    # TODO: Read DNA sequence file into a variable

            # Method 1 in reading sequence from  , This method will only read the first line of the file 'f2' (sequence file),
            #  as if you open the file itself you will notice that the file actually contain one line only
            sequence = f2.readline()

            # Method 2 in reading sequence from file, Here I used DiceReader(), then used next() to read the next row/line (that will be the first,
            # if that was the first time to call next()), then I used join() which is going to convert the return value of next() (which is a list) to 'str'
            # sequence_reader = csv.reader(f2)
            # sequence = ''.join(next(sequence_reader))

            # This will store the very first line of the dictReader() object 'database_reader' in a list called field_names,
            #  (notice that the very first line is going to be the KEYS used in the 'databas_reader' object in its key-value pairs),
            #  and those keys contain the 'STRs' that I want to look for there 'longest_runs' in the sequence file
            field_names = database_reader.fieldnames

            # Iterate over the field names/(keys)/STR, then assign them to the variable called sequence, notice that I started from 1, because I don't want to store the 'name' field
            for i in range(1, len(field_names)):
                subsequence = field_names[i]

    # TODO: Find longest match of each STR in DNA sequence

                # The already made up logest_run function created by cs50, which is used to return the longest_run of each subsequence (STR) found sequence file
                longest_run = longest_match(sequence, subsequence)

                # Add each longest run of a specific STR/subsequence to a list, which will store at the end the longest_runs of every STR/subsequence found the sequence file
                longest_run_list.append(longest_run)

    # TODO: Check database for matching profiles

            # Iterate over each row found in 'database_reader' object, then create a list of all the values found in the dictionary located in that very 'row'
            for row in database_reader:
                suspect = list(row.values())
                # Remove the first elemnt because its the value of 'name' field/key, an we want to store only the subsequences/STRs
                del suspect[0]

                # Method 1:: To covert dna STRs of persons found in database file form a 'str' to 'Int'
                # suspect = list(map(int, suspect))

                # Method 2:: To covert dna STRs of persons found in database file form a 'str' to 'Int'
                suspect = [int(x) for x in suspect]

                # If there is a match, exit with a message containing the persons name
                if suspect == longest_run_list:
                    sys.exit(row['name'])

            # If there isn't a match with any of the persons data found in database file then exit with message indicating that there is no match found !
            sys.exit('No match')



def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
