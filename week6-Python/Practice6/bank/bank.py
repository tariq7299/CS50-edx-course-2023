 #!/usr/bin/env python3

import sys
import py_compile


def main():
    greeting = input('Greeting: ')
    greeting_words_list = greeting.strip().lower().split()

    greeting_word = greeting_words_list[0]

    if greeting_word == 'hello' or greeting_word == 'hello,':
        print('0$')
    elif greeting_word[0] == 'h' or greeting_word[0] == 'H':
        print('20$')
    else:
        print('100$')



main()