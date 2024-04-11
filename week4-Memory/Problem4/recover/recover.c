#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    // change name "raw"
    FILE *raw = fopen(argv[1], "r");

    if (raw == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }

    // Counter of sprintf(), used to create file from 000.jpg to 049.jpg
    int n = 0;

    // Creation of a Custom name "BYTE" which represent a unsigned 8 bit intger , to hold/store the bytes of recovered images
    typedef uint8_t BYTE;

    //To make 'img' pointer of the recovered images files, to be able to write 'fclose(img)' at the end
    // , because if created it inside of 'if' functions it wont be able to write fclose() at the end as 'img' pointer will be local to them
    FILE *img = NULL;
    char *filename = malloc(sizeof(char)*8);

    // This makes the variable 'buffer' holds 1 block(512 BYTES) each time we write into it from 'raw', as the files stored inside raw are blocks of 512 BYTE
    BYTE *buffer = malloc(sizeof(BYTE)*512);

    // Reads 1 block each time until it finishes reading all of the blocks inside raw
    while (fread(buffer, sizeof(BYTE), 512, raw) == 512)
    {
        // Checks whether the block is the beginning of a new jpeg file/image
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Checks whether this is the first jpeg File
            if (n == 0)
            {
                sprintf(filename, "%03i.jpg", n);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not open file 'img'");
                    return 3;
                }
                fwrite(buffer, sizeof(BYTE), 512, img);
                n++;
            }
            // If it wasn't the first jpeg file so it has to be different one (like the fifth, 20, 15, 45......etc)
            else if (n > 0)
            {
                // we  are closing "img" because we detected from the "if" statment we are currently in, that this is new jpeg file, so we need to close the previous one and open/create a new one
                fclose(img);
                sprintf(filename, "%03i.jpg", n);
                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Could not open file 'img'");
                    return 3;
                }
                fwrite(buffer, sizeof(BYTE), 512, img);
                n++;
            }
        }
        // So here we know that the next block (buffer)/(512 BYTES) is not a start of a new jpeg File,m so need to continue writing to one are cuurently in untill we find/detect/stop at new jpeg file inside 'raw'
        else if (n > 0)
        {
            // Here we will not Close any file beacuse we want to write at the cuurent file
            // Also we will no increment 'n' beacause we need to write from raw into the same img file until we encounter a new jpeg file
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
    }

fclose(raw);
fclose(img);
free(buffer);
free(filename);
}



