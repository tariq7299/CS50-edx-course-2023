import sys
import random
from pyfiglet import Figlet, FigletFont
# from pyfiglet import FigletFont



# print (pyfiglet.FigletFont.getFonts())
def main():

    if len(sys.argv) == 3:
        if sys.argv[1] == '-f' or sys.argv[1] == '--font':
            Font_name = sys.argv[2]
            for FONT in FigletFont.getFonts():
                if FONT == Font_name:
                    Text = input('Input: ')
                    f = Figlet(font=Font_name)
                    print(f.renderText(Text))
                    return 0
            sys.exit('Invalid usage')
        else:
            sys.exit('Invalid usage')
    elif len(sys.argv) == 1:
        Font_name = random.choices(FigletFont.getFonts(), weights =None, k = 1)
        Text = input('Input: ')
        f = Figlet(font=Font_name[0])
        print(f.renderText(Text))
    else:
            sys.exit('Invalid usage')




main()