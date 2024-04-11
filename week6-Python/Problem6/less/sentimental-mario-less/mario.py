# TODO


def main():

    hight = Get_Hight()
    print_pyramid(hight)



def Get_Hight():

        # A recurcive function that its job to keep prompting the user until he gives a valid hight (By me !)
        try:
            hight = int(input('Hight: '))
            if hight in range(1, 9):
                return hight
            else:
                print("Invalid Hight !, Please Enter a number not more than 8")
                return Get_Hight()
        except ValueError :
            print("Invalid Hight !, Please Enter a number not more than 8")
            return Get_Hight()

# Another way to keep prompt the user for a valid hight
    # while True:
    #     try:
    #         hight = int(input('Hight: '))
    #         if hight in range(9):
    #         # if 0 <= hight => 8:
    #             break
    #         else:
    #             print("Invalid Hight !, Please Enter a number not more than 8")
    #     except ValueError :
    #         print("Invalid Hight !, Please Enter a number not more than 8")


def print_pyramid(height):




    # # # First method
    # for i in range(height-1, -1, -1):
    #         for p in range(i-1, -1, -1):
    #             print(' ', end='')
    #         for j in range(height-i):
    #             print('#', end='')
    #         print()

# # First method (a littel bit different)
    for i in range(height, 0, -1):
            for p in range(i, 1, -1):
                print(' ', end='')
            for j in range(height-i+1):
                print('#', end='')
            print()

    # # # First method
    # for i in range(height-1, -1, -1):
    #         for p in range(i-1, -1, -1):
    #             print(' ', end='')
    #         for j in range(height-i):
    #             print('#', end='')
    #         print()
    # Second method
    # for i in range(height):
    #     spaces = " " * (height - i - 1)
    #     blocks = "#" * (i + 1)
    #     print(f"{spaces}{blocks}")

    # Third Method
    # for i in range(height):
    #     for j in range(height):
    #         if i + j < height - 1:
    #             print(" ", end="")
    #         else:
    #             print("#", end="")
    #     print()

    # A method which create an inverse pyramid
    # for i in range(height, 0, -1):
    #         for p in range(i, 0, -1):
    #             print('#', end='')
    #         print()

main()