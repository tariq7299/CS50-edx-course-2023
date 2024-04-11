// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file


    uint8_t header[HEADER_SIZE];

    // First method by using "fread()" & "fwrite()"
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);

    // Another Method to copy "header[]" to "*output" by using "fgetc()" and "fputc()"
    /*
     int i = 0;
     while(ftell(input) < 44 )
     {
         header[i] = fgetc(input);
         fputc(header[i], output);
         i++;
     }
     */

    // TODO: Read samples from input file and write updated data to output file
    fseek(input, 44, SEEK_SET);

    int16_t buffer;

    // This method didn't work correctly as it reads one more buffer after the loop actually reaches the end of the file. So, if you open the audio, it seems that it was tuned up/down correctly, and you won't notice that tiny problem at all. However, CHECK50 will, and it won't accept it.
    // while(feof(input) == 0)
    // {
    //     fread(&buffer, sizeof(int16_t), 1, input);
    //     buffer = buffer * factor;
    //     fwrite(&buffer, sizeof(int16_t), 1, output);
    // }

    // Another method by using "feof()" as inside "while" statment, which will check if we reach the end-of-file
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        buffer = buffer * factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }


    // Close files
    fclose(input);
    fclose(output);
}
