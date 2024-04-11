# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000



def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file

    with open(sys.argv[1], 'r') as f:

        # # First Method to read the file 'f' as a dictReader object then transfer' team[rating]' from a 'string' to 'Int',
        # here I stored the return of DictReader() in 'teams[]' directly (but after converting the retun value of 'DictReader' to a list [])then changed team[rating] to Int
        teams = list(csv.DictReader(f))
        # A 'for' loop which will go through each element in 'teams' untile the end of its elements,
        for i in range(len(teams)):
            # I am converting the number (rating) stored as a 'string' that is the value of the key 'teams['rating']',
            #  to 'Int' date type, in order to be able to do calculations on it later in code
            teams[i]['rating'] = int(teams[i]['rating'])

        # # Second Method to read the file 'f' as a dictReader object then transfer 'team[rating]' from a 'string' to 'Int'
        # here I stored DictReader() in seperate variable called 'reader' then tranfered each team[rating] then appended it to teams list
        # reader = csv.DictReader(f)
        # for team in reader:
        #     team['rating'] = int(team['rating'])
        #     teams.append(team)



    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    # To do it N times
    for i in range(N):
        # The return value is he name of winner team
        team_name = simulate_tournament(teams)
        # If team wins before it adds another win to its win count !
        if team_name in counts:
            counts[team_name] += 1
        # However if it the first time to win, then it assing '1' (first win) as a value to the new key 'team_name'
        else:
            counts[team_name] = 1



    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    # Very wierd that there is no word "if" here !!
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    # A recurcive function !, whose its base is ((if teams becomes a lenght of one, which also means that there is only one left, and that is the winner !))
    # So this function will keep going/repeating untill there is only one team left inside of teams []
    if len(teams) == 1:
        return teams[0]['team']
    else:
        return simulate_tournament(simulate_round(teams))



 
    main()
