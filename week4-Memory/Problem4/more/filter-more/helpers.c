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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    float Gx_Value, Gy_Value;
    int i, m;
    float RedMiddle, RedMiddleLeft, RedMiddleRight, RedAbove, RedAboveLeft, RedAboveRight, RedBelow, RedBelowLeft, RedBelowRight
    , GreenMiddle, GreenMiddleLeft, GreenMiddleRight, GreenAbove, GreenAboveLeft, GreenAboveRight, GreenBelow, GreenBelowLeft, GreenBelowRight
    , BlueMiddle, BlueMiddleLeft, BlueMiddleRight, BlueAbove, BlueAboveLeft, BlueAboveRight, BlueBelow, BlueBelowLeft, BlueBelowRight;

    // This one of the ways to dynamilcy create/allocate two dimentional array
    // This creats a two dimentional array calles "image_temp[][]" it is excalty the same size as "image[][]"
    RGBTRIPLE(*image_temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    if (image_temp == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }

     for (i = 0; i < height; i ++)
    {
        for (m = 0; m < width; m++)
        {
            RedMiddle = image[i][m].rgbtRed;
            RedAboveLeft = image[i-1][m-1].rgbtRed;
            RedAbove = image[i-1][m].rgbtRed;
            RedAboveRight = image[i-1][m+1].rgbtRed;
            RedMiddleLeft = image[i][m-1].rgbtRed;
            RedMiddleRight = image[i][m+1].rgbtRed;
            RedBelowLeft = image[i+1][m-1].rgbtRed;
            RedBelow = image[i+1][m].rgbtRed;
            RedBelowRight = image[i+1][m+1].rgbtRed;

            GreenMiddle = image[i][m].rgbtGreen;
            GreenAboveLeft = image[i-1][m-1].rgbtGreen;
            GreenAbove = image[i-1][m].rgbtGreen;
            GreenAboveRight = image[i-1][m+1].rgbtGreen;
            GreenMiddleLeft = image[i][m-1].rgbtGreen;
            GreenMiddleRight = image[i][m+1].rgbtGreen;
            GreenBelowLeft = image[i+1][m-1].rgbtGreen;
            GreenBelow = image[i+1][m].rgbtGreen;
            GreenBelowRight = image[i+1][m+1].rgbtGreen;

            BlueMiddle = image[i][m].rgbtBlue;
            BlueAboveLeft = image[i-1][m-1].rgbtBlue;
            BlueAbove = image[i-1][m].rgbtBlue;
            BlueAboveRight = image[i-1][m+1].rgbtBlue;
            BlueMiddleLeft = image[i][m-1].rgbtBlue;
            BlueMiddleRight = image[i][m+1].rgbtBlue;
            BlueBelowLeft = image[i+1][m-1].rgbtBlue;
            BlueBelow = image[i+1][m].rgbtBlue;
            BlueBelowRight = image[i+1][m+1].rgbtBlue;

            int RED_value, Green_value, Blue_value;

            if (i == 0 || m == 0 || i == (height-1) || m == (width-1))
            {

                if (i == 0 && m == 0)
                {
                    RedAbove = 0;
                    RedAboveRight = 0;
                    RedAboveLeft = 0;
                    RedMiddleLeft = 0;
                    RedBelowLeft = 0;

                    GreenAbove = 0;
                    GreenAboveRight = 0;
                    GreenAboveLeft = 0;
                    GreenMiddleLeft = 0;
                    GreenBelowLeft = 0;

                    BlueAbove = 0;
                    BlueAboveRight = 0;
                    BlueAboveLeft = 0;
                    BlueMiddleLeft = 0;
                    BlueBelowLeft = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i == 0 && m == (width-1))
                {
                    RedAboveLeft = 0;
                    RedAbove = 0;
                    RedAboveRight = 0;
                    RedMiddleRight = 0;
                    RedBelowRight = 0;

                    GreenAboveLeft = 0;
                    GreenAbove = 0;
                    GreenAboveRight = 0;
                    GreenMiddleRight = 0;
                    GreenBelowRight = 0;

                    BlueAboveLeft = 0;
                    BlueAbove = 0;
                    BlueAboveRight = 0;
                    BlueMiddleRight = 0;
                    BlueBelowRight = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i == (height-1) && m == (width-1))
                {
                    RedAboveRight = 0;
                    RedMiddleRight = 0;
                    RedBelowRight = 0;
                    RedBelow = 0;
                    RedBelowLeft = 0;

                    GreenAboveRight = 0;
                    GreenMiddleRight = 0;
                    GreenBelowRight = 0;
                    GreenBelow = 0;
                    GreenBelowLeft = 0;

                    BlueAboveRight = 0;
                    BlueMiddleRight = 0;
                    BlueBelowRight = 0;
                    BlueBelow = 0;
                    BlueBelowLeft = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i == (height-1) && m == 0)
                {
                    RedAboveLeft = 0;
                    RedMiddleLeft = 0;
                    RedBelowLeft = 0;
                    RedBelow = 0;
                    RedBelowRight = 0;

                    GreenAboveLeft = 0;
                    GreenMiddleLeft = 0;
                    GreenBelowLeft = 0;
                    GreenBelow = 0;
                    GreenBelowRight = 0;

                    BlueAboveLeft = 0;
                    BlueMiddleLeft = 0;
                    BlueBelowLeft = 0;
                    BlueBelow = 0;
                    BlueBelowRight = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    image_temp[i][m].rgbtGreen = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i != 0 && i != (height-1) && m == 0)
                {

                    RedAboveLeft = 0;
                    RedMiddleLeft = 0;
                    RedBelowLeft = 0;

                    GreenAboveLeft = 0;
                    GreenMiddleLeft = 0;
                    GreenBelowLeft = 0;

                    BlueAboveLeft = 0;
                    BlueMiddleLeft = 0;
                    BlueBelowLeft = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if ( i != 0 && i != (height-1) && m == (width-1))
                {
                    RedAboveRight = 0;
                    RedMiddleRight = 0;
                    RedBelowRight = 0;

                    GreenAboveRight = 0;
                    GreenMiddleRight = 0;
                    GreenBelowRight = 0;

                    BlueAboveRight = 0;
                    BlueMiddleRight = 0;
                    BlueBelowRight = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i == 0 && m != 0 && m != (width-1))
                {
                    RedAboveLeft = 0;
                    RedAbove = 0;
                    RedAboveRight = 0;

                    GreenAboveLeft = 0;
                    GreenAbove = 0;
                    GreenAboveRight = 0;

                    BlueAboveLeft = 0;
                    BlueAbove = 0;
                    BlueAboveRight = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
                else if (i == (height-1) && m != 0 && m != (width-1))
                {
                    RedBelowLeft = 0;
                    RedBelow = 0;
                    RedBelowRight = 0;

                    GreenBelowLeft = 0;
                    GreenBelow = 0;
                    GreenBelowRight = 0;

                    BlueBelowLeft = 0;
                    BlueBelow = 0;
                    BlueBelowRight = 0;

                    Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                    * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                    Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                    * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                    RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (RED_value > 255)
                    {
                        image_temp[i][m].rgbtRed = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtRed = RED_value;
                    }

                    Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                    * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                    Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                    * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                    Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Green_value > 255)
                    {
                        image_temp[i][m].rgbtGreen = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtGreen = Green_value;
                    }

                    Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                    * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                    Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                    * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                    Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                    if (Blue_value > 255)
                    {
                        image_temp[i][m].rgbtBlue = 255;
                    }
                    else
                    {
                    image_temp[i][m].rgbtBlue = Blue_value;
                    }
                }
            }
            else
            {
                Gx_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * 0 + RedAboveRight * 1 + RedMiddleLeft * -2 + RedMiddleRight
                * 2 + RedBelowLeft * -1 + RedBelow * 0 + RedBelowRight * 1);
                Gy_Value = (RedMiddle * 0 + RedAboveLeft * -1 + RedAbove * -2 + RedAboveRight * -1 + RedMiddleLeft * 0 + RedMiddleRight
                * 0 + RedBelowLeft * 1 + RedBelow * 2 + RedBelowRight * 1);
                RED_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                if (RED_value > 255)
                {
                    image_temp[i][m].rgbtRed = 255;
                }
                else
                {
                image_temp[i][m].rgbtRed = RED_value;
                }

                Gx_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * 0 + GreenAboveRight * 1 + GreenMiddleLeft * -2 + GreenMiddleRight
                * 2 + GreenBelowLeft * -1 + GreenBelow * 0 + GreenBelowRight * 1);
                Gy_Value = (GreenMiddle * 0 + GreenAboveLeft * -1 + GreenAbove * -2 + GreenAboveRight * -1 + GreenMiddleLeft * 0 + GreenMiddleRight
                * 0 + GreenBelowLeft * 1 + GreenBelow * 2 + GreenBelowRight * 1);
                Green_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                if (Green_value > 255)
                {
                    image_temp[i][m].rgbtGreen = 255;
                }
                else
                {
                image_temp[i][m].rgbtGreen = Green_value;
                }

                Gx_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * 0 + BlueAboveRight * 1 + BlueMiddleLeft * -2 + BlueMiddleRight
                * 2 + BlueBelowLeft * -1 + BlueBelow * 0 + BlueBelowRight * 1);
                Gy_Value = (BlueMiddle * 0 + BlueAboveLeft * -1 + BlueAbove * -2 + BlueAboveRight * -1 + BlueMiddleLeft * 0 + BlueMiddleRight
                * 0 + BlueBelowLeft * 1 + BlueBelow * 2 + BlueBelowRight * 1);
                Blue_value = round(sqrt(Gx_Value * Gx_Value + Gy_Value * Gy_Value));
                if (Blue_value > 255)
                {
                    image_temp[i][m].rgbtBlue = 255;
                }
                else
                {
                image_temp[i][m].rgbtBlue = Blue_value;
                }
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



