# TODO


def main():
    # A prompt to the user which stores his input in variable called 'text'
    text = input('Text: ')
    # 't 'variable which will be an object of the class 'Text()'
    t = Text(text)

    # Stores the number of letters found in the "text" in "num_letters"
    num_letters = t.count_letters()

    # Stores the number of words found inside "text" in "num_words"
    num_words = t.count_words()

    # Stores the number of scentences found inside "text: in "num_scentences"
    num_scentences = t.count_scentences()

    # A function which calculates the grade level of the text using Coleman-Liau formula, then it prints it to screen
    calculate_grade_level(num_letters, num_words, num_scentences)



# A class 'Text' which has methods to calculate number of letters/word/scentences
class Text:


    def __init__(self, text):
        self.text = text
        self.num_letters = 0
        self.num_words = 0
        self.num_scentences = 0


    def count_letters(self):

        # It loops over each character found in "text"
        for char in self.text:
            # If the isalpha() function returns a non-zero int then that character is alphabetical, so it increments "num_letters" by one
            if char.isalpha():
                self.num_letters += 1
        return self.num_letters

    def count_words(self):
        # I assigned the seperator(argument) in split() to be a 'space'/' ', so it will split words whenever it encounters a ' '/'space'
        words = self.text.split(' ')
        # Then i used len() to count the number of elements(words) in the list 'words[]'
        self.num_words = len(words)
        return self.num_words

    def count_scentences(self):
        # This set contain a bunch of characters each resembles an end of a scentence character
        end_of_scentences_char = {'!', '.', '?'}
        # Loops over each character found in "text" (the text provided by user), then it tests if it is a an end of scentences character, finaly it increments "num_scentences" by 1,
        #  (which means in another words that we just finished a scentence)
        for char in self.text:
            if char in end_of_scentences_char:
                self.num_scentences += 1
        return self.num_scentences



def calculate_grade_level(num_letters, num_words, num_scentences):

    # Coleman-Liau formula,

    #  "L" ratio of the number of letters found in the whole text to the number of words found in the whole text
    L = float(num_letters) / float(num_words) * 100

    #  "S" ratio of the number of scentences found in the whole text to the number of words found in the whole text
    S = float(num_scentences) / float(num_words) * 100

    index = 0.0588 * L - 0.296 * S - 15.8

    if index > 16:
        print('Grade 16+')
    elif index < 1:
        print('Before Grade 1')
    else:
        print(f'Grade {round(index)}')








main()