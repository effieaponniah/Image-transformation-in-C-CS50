// This program is written by Effiea Ponniah
// This program is Filters and converts an image to grayscale, reflects, blures and denotes the edges.
// Written on October 16th, 2020
// Modified on December 15th, 2020

#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE my_pixel;
    int avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            my_pixel = image[i][j];
            avg = (round)((my_pixel.rgbtRed + my_pixel.rgbtBlue + my_pixel.rgbtGreen) / 3.0);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}


// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE my_pixel;
    int temp_red;
    int temp_blue;
    int temp_green;

    // Image rows and column & Algorithm to iterate to switch pixels horizontally
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Store a pixel in a temp variable before replacing
            temp_red  =  image[i][j].rgbtRed;
            temp_blue  =  image[i][j].rgbtBlue;
            temp_green  =  image[i][j].rgbtGreen;

            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;

            image[i][width - j - 1].rgbtRed = temp_red;
            image[i][width - j - 1].rgbtBlue = temp_blue;
            image[i][width - j - 1].rgbtGreen = temp_green;
        }
    }
    return;
}


// Blur image
// Taking each pixel and, for each color value, giving it a new value
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Duplicate the image and take a copy of it
    RGBTRIPLE duplicate_img[height][width];

    int sum_red, sum_blue, sum_green;
    float divisor;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum_red = 0;
            sum_blue = 0;
            sum_green = 0;
            divisor = 0.00;

            // So I am doing one row above and one row below the center of axis cell
            for (int y = -1; y < 2; y++)
            {
                // So one column left and one column right of the center cell
                for (int x = -1; x < 2; x++)
                {
                    if (i + y < 0 || i + y > height - 1 || j + x < 0 || j + x > width - 1)
                    {
                        continue;
                    }

                    RGBTRIPLE pix = image[i + y][j + x];

                    sum_red += pix.rgbtRed;
                    sum_blue += pix.rgbtBlue;
                    sum_green += pix.rgbtGreen;
                    divisor++;
                }
            }

            // Averaging the color values of neighboring pixels
            duplicate_img[i][j].rgbtRed = round(sum_red / divisor);
            duplicate_img[i][j].rgbtBlue = round(sum_blue / divisor);
            duplicate_img[i][j].rgbtGreen = round(sum_green / divisor);
        }
    }

    // Load duplicate image back to original
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed  = duplicate_img[i][j].rgbtRed;
            image[i][j].rgbtBlue  = duplicate_img[i][j].rgbtBlue;
            image[i][j].rgbtGreen  = duplicate_img[i][j].rgbtGreen;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE duplicate_img[height][width];

    // Modifying a pixel based on below 3x3 grid of pixels that surrounds that pixel
    int Gx[3][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {{-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    float gx_red, gx_blue, gx_green;
    float gy_red, gy_blue, gy_green;
    float g_red, g_blue, g_green;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gx_red = 0;
            gx_blue = 0;
            gx_green = 0;

            gy_red = 0;
            gy_blue = 0;
            gy_green = 0;

            // So I am doing one row above and one row below the center of axis cell
            for (int y = -1; y < 2; y++)
            {
                // So one column left and one column right of the center cell
                for (int x = -1; x < 2; x++)
                {
                    if (i + y < 0 || i + y > height - 1 || j + x < 0 || j + x > width - 1)
                    {
                        continue;
                    }

                    //printf("Values: %d %d %d %d\n", i, j, x, y);

                    RGBTRIPLE pix = image[i + y][j + x];
                    gx_red += Gx[y + 1][x + 1] * pix.rgbtRed;
                    gx_blue += Gx[y + 1][x + 1] * pix.rgbtBlue;
                    gx_green += Gx[y + 1][x + 1] * pix.rgbtGreen;

                    gy_red += Gy[y + 1][x + 1] * pix.rgbtRed;
                    gy_blue += Gy[y + 1][x + 1] * pix.rgbtBlue;
                    gy_green += Gy[y + 1][x + 1] * pix.rgbtGreen;
                }
            }

            // Calculating using Sobel filter algorithm
            g_red = round(sqrt(gx_red * gx_red + gy_red * gy_red));
            g_blue = round(sqrt(gx_blue * gx_blue + gy_blue * gy_blue));
            g_green = round(sqrt(gx_green * gx_green + gy_green * gy_green));

            duplicate_img[i][j].rgbtRed = g_red > 255 ? 255 : g_red;
            duplicate_img[i][j].rgbtBlue = g_blue > 255 ? 255 : g_blue;
            duplicate_img[i][j].rgbtGreen = g_green > 255 ? 255 : g_green;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed  = duplicate_img[i][j].rgbtRed;
            image[i][j].rgbtBlue  = duplicate_img[i][j].rgbtBlue;
            image[i][j].rgbtGreen  = duplicate_img[i][j].rgbtGreen;
        }
    }
    return;
}
