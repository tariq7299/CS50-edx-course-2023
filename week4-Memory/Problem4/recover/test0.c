#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    // change name "card"
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    // cahnge name
    int n = 0;
    typedef uint8_t BYTE;
    // typedef 512*BYTE BLOCK;

    char *filename = malloc(sizeof(char)*8);
    BYTE *buffer = malloc(sizeof(BYTE)*512);
    // change name of x
    long int x = 0;

    while (fread(buffer, sizeof(BYTE), 512, card) == 512)
    {
    // while(feof(card) == 0)
    // {
        // fseek(card, x, SEEK_SET);
        // printf("x0 : %i\n", x);
        // fread(buffer, sizeof(BYTE), 512, card);
        if (n == 0)
        {
            if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
            {
                sprintf(filename, "%03i.jpg", n);
                FILE *img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not open file 'img'");
                    return 2;
                }
                do
                {
                    fwrite(buffer, sizeof(BYTE), 512, img);
                    fread(buffer, sizeof(BYTE), 512, card);
                    // x = ftell(card);
                    // printf("x1 : %i\n", x);
                }
                while (buffer[0] != 0xff && buffer[1] != 0xd8 && buffer[2] != 0xff && (buffer[3] & 0xf0) != 0xe0 && feof(card) == 0);
                n++;
                fclose(img);
            }
        }
        else if (n > 0)
        {
            sprintf(filename, "%03i.jpg", n);
            FILE *img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not open file 'img'");
                return 2;
            }
            do
            {
                fwrite(buffer, sizeof(BYTE), 512, img);
                fread(buffer, sizeof(BYTE), 512, card);
                // x = ftell(card);
                // printf("x1 : %i\n", x);
            }
            while (buffer[0] != 0xff && buffer[1] != 0xd8 && buffer[2] != 0xff && (buffer[3] & 0xf0) != 0xe0 && feof(card) == 0);
            n++;
            fclose(img);
        }
    }

fclose(card);
// fclose(img);
free(buffer);
free(filename);
}



