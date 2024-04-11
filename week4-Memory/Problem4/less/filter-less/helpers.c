#include "helpers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Goes through every ROW
    int i, m;
    for (i = 0; i < height; i ++)
    {
        // Goes through every COLUMN
        for (m = 0; m < width; m++)
        {
            // Takes every color value from each pixel, then calculate the average of them
            float BLU = image[i][m].rgbtBlue;
            float GREEN = image[i][m].rgbtGreen;
            float RED = image[i][m].rgbtRed;
            float AVERAGE_or_shadeofgrey = (BLU + GREEN + RED)/3.0;
            // USes "round()" function to round numbers to nearest integer
            image[i][m].rgbtBlue = round(AVERAGE_or_shadeofgrey);
            image[i][m].rgbtGreen = round(AVERAGE_or_shadeofgrey);
            image[i][m].rgbtRed = round(AVERAGE_or_shadeofgrey);
        }

    }


    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    int i, m;
    for (i = 0; i < height; i ++)
    {
        for (m = 0; m < width; m++)
        {
            // Implements a given formula which output speia filter (specific light brownesh color) for every pixel

            float sepiaRed = .393 * image[i][m].rgbtRed + .769 * image[i][m].rgbtGreen + .189 * image[i][m].rgbtBlue;
            sepiaRed = round(sepiaRed);
            /* if he calculated color value was more than 255 bit, it will be equal to 255, because "image[][].rgbt"color"" as BYTE data type
            wihch stores a maximum of 255 bits (1 byte), so if it exceeds 255 it will start over from 0, so if it was 258 it will be stored as 2*/
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            float sepiaGreen = .349 * image[i][m].rgbtRed + .686 * image[i][m].rgbtGreen + .168 * image[i][m].rgbtBlue;
            sepiaGreen = round(sepiaGreen);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            float sepiaBlue = .272 * image[i][m].rgbtRed + .534 * image[i][m].rgbtGreen + .131 * image[i][m].rgbtBlue;
            sepiaBlue = round(sepiaBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][m].rgbtRed = sepiaRed;
            image[i][m].rgbtGreen = sepiaGreen;
            image[i][m].rgbtBlue = sepiaBlue;
        }

    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int i, m, temp;
    for (i = 0; i < height; i++)
    {
        // ""m < width / 2"" to stop at middle horizontal axis
        for (m = 0; m < width / 2; m++)
        {
            // Replaces both the right side and the left side of the pic
            temp = image[i][m].rgbtRed;

            image[i][m].rgbtRed = image[i][width-1-m].rgbtRed;
            image[i][width-1-m].rgbtRed = temp;

            temp = image[i][m].rgbtGreen;
            image[i][m].rgbtGreen = image[i][width-1-m].rgbtGreen;
            image[i][width-1-m].rgbtGreen = temp;

            temp = image[i][m].rgbtBlue;
            image[i][m].rgbtBlue = image[i][width-1-m].rgbtBlue;
            image[i][width-1-m].rgbtBlue = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // This one of the ways to dynamilcy create/allocate two dimentional array
    // This creats a tow dimentional array calles "image_temp[][]" it is excalty the same size as "image[][]"
    RGBTRIPLE(*image_temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    if (image_temp == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }
   int i, m;
    for (i = 0; i < height; i ++)
    {
        for (m = 0; m < width; m++)
        {
            /* We have to make a special case conditions for both corner pixels, and edge pixels, because it is different in:

            1- the divisor is differnet between normal middle pixels(9) and corner pixels(4), and pixles on edges(6)
            2- The location of the pixel dtermines the values of the array image[x][y], and we cant make it all the same formula, because in corner pixles
            cant accept -1 like m-1 or i -1, becuse if "i" equals to zero, i-1 will be equals to -1, then it will result in an error, as you are accessing a
            location in memory out of boundries, also it wont (sometimes) accept i+1 as "i" can eqaul to height-1 then if then i+1 will be equal to "height",
            and this also a place out of the picture boundries, so it will result (somtimes) an "error" (segmentation fault).

            So for the previous reasons, we have to make a fromula/case for each catogery/place in the picture.

            // Blurs both Corner pixles, and Edge pixels,
            */
            if (i == 0 || m == 0 || i == (height-1) || m == (width-1))
            {

                // Blurs Top Left pixel
                if (i == 0 && m == 0)
                {
                    // I used a temporary array called "image_temp[i][m].rgbtColor" to store the color values, then at the end i copy this array to the
                    // original, all that to not mess up my color values,which was going to happen if we use the same original array when calculating the blur values
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i][m+1].rgbtRed + image[i+1][m+1].rgbtRed + image[i+1][m].rgbtRed)/4.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i][m+1].rgbtGreen + image[i+1][m+1].rgbtGreen + image[i+1][m].rgbtGreen)/4.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i][m+1].rgbtBlue + image[i+1][m+1].rgbtBlue + image[i+1][m].rgbtBlue)/4.0);

                }
                // Blurs Top right pixel
                else if (i == 0 && m == (width-1))
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i][m-1].rgbtRed + image[i+1][m-1].rgbtRed + image[i+1][m].rgbtRed)/4.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i][m-1].rgbtGreen + image[i+1][m-1].rgbtGreen + image[i+1][m].rgbtGreen)/4.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i][m-1].rgbtBlue + image[i+1][m-1].rgbtBlue + image[i+1][m].rgbtBlue)/4.0);
                }
                // Blurs bottom right pixel
               else if (i == (height-1) && m == (width-1))
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m-1].rgbtRed + image[i][m-1].rgbtRed)/4.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m-1].rgbtGreen + image[i][m-1].rgbtGreen)/4.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m-1].rgbtBlue + image[i][m-1].rgbtBlue)/4.0);
                }
                // Blurs bottom left pixel
                else if (i == (height-1) && m == 0)
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m+1].rgbtRed + image[i][m+1].rgbtRed)/4.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m+1].rgbtGreen + image[i][m+1].rgbtGreen)/4.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m+1].rgbtBlue + image[i][m+1].rgbtBlue)/4.0);
                }
                //Pixels on Edges Excluding all 4 corner pixels
                //Blurs Left Edeg Column
                 else if (i != 0 && i != (height-1) && m == 0)
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m+1].rgbtRed + image[i][m+1].rgbtRed + image[i+1][m+1].rgbtRed + image[i+1][m].rgbtRed)/6.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m+1].rgbtGreen + image[i][m+1].rgbtGreen + image[i+1][m+1].rgbtGreen + image[i+1][m].rgbtGreen)/6.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m+1].rgbtBlue + image[i][m+1].rgbtBlue + image[i+1][m+1].rgbtBlue + image[i+1][m].rgbtBlue)/6.0);
                }
                // Blurs Right Edge Column
                else if ( i != 0 && i != (height-1) && m == (width-1))
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m-1].rgbtRed + image[i][m-1].rgbtRed + image[i+1][m-1].rgbtRed + image[i+1][m].rgbtRed)/6.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m-1].rgbtGreen + image[i][m-1].rgbtGreen + image[i+1][m-1].rgbtGreen + image[i+1][m].rgbtGreen)/6.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m-1].rgbtBlue + image[i][m-1].rgbtBlue + image[i+1][m-1].rgbtBlue + image[i+1][m].rgbtBlue)/6.0);
                }
                // Blurs Top Edge Row
                else if (i == 0 && m != 0 && m != (width-1))
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i][m+1].rgbtRed + image[i+1][m+1].rgbtRed + image[i+1][m].rgbtRed + image[i+1][m-1].rgbtRed + image[i][m-1].rgbtRed)/6.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i][m+1].rgbtGreen + image[i+1][m+1].rgbtGreen + image[i+1][m].rgbtGreen + image[i+1][m-1].rgbtGreen + image[i][m-1].rgbtGreen)/6.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i][m+1].rgbtBlue + image[i+1][m+1].rgbtBlue + image[i+1][m].rgbtBlue + image[i+1][m-1].rgbtBlue + image[i][m-1].rgbtBlue)/6.0);
                }
                // Blurs bottom Edge Row
                else if (i == (height-1) && m != 0 && m != (width-1))
                {
                    image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i][m-1].rgbtRed + image[i-1][m-1].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m+1].rgbtRed + image[i][m+1].rgbtRed)/6.0);
                    image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i][m-1].rgbtGreen + image[i-1][m-1].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m+1].rgbtGreen + image[i][m+1].rgbtGreen)/6.0);
                    image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i][m-1].rgbtBlue + image[i-1][m-1].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m+1].rgbtBlue + image[i][m+1].rgbtBlue)/6.0);
                }
            }
            else
            {
                image_temp[i][m].rgbtRed = round((image[i][m].rgbtRed + image[i-1][m-1].rgbtRed + image[i-1][m].rgbtRed + image[i-1][m+1].rgbtRed + image[i][m-1].rgbtRed + image[i][m+1].rgbtRed
                + image[i+1][m-1].rgbtRed + image[i+1][m].rgbtRed + image[i+1][m+1].rgbtRed)/9.0);

                image_temp[i][m].rgbtGreen = round((image[i][m].rgbtGreen + image[i-1][m-1].rgbtGreen + image[i-1][m].rgbtGreen + image[i-1][m+1].rgbtGreen + image[i][m-1].rgbtGreen + image[i][m+1].rgbtGreen
                + image[i+1][m-1].rgbtGreen + image[i+1][m].rgbtGreen + image[i+1][m+1].rgbtGreen)/9.0);

                image_temp[i][m].rgbtBlue = round((image[i][m].rgbtBlue + image[i-1][m-1].rgbtBlue + image[i-1][m].rgbtBlue + image[i-1][m+1].rgbtBlue + image[i][m-1].rgbtBlue + image[i][m+1].rgbtBlue
                + image[i+1][m-1].rgbtBlue + image[i+1][m].rgbtBlue + image[i+1][m+1].rgbtBlue)/9.0);

            }


        }
    }

    // Finaly we copies the content of "image_temp[i][m].rgbtcolor" array to "image[i][m].rgbtcolor"
    for (i = 0; i < height; i ++)
    {
        for (m = 0; m < width; m++)
        {
            image[i][m].rgbtRed = image_temp[i][m].rgbtRed;
            image[i][m].rgbtGreen = image_temp[i][m].rgbtGreen;
            image[i][m].rgbtBlue = image_temp[i][m].rgbtBlue;
        }
        }
        free(image_temp);
    return;
}
