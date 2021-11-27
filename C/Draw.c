// Student stub code for ASCII Drawing assignment

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 * Name:        Ryan Cole
 * Username:    rccole@mtu.edu
 * Description: This program creates an image drawn using ASCII characters from a sequence of instructions
 *              read from standard input. The image is stored in a 2D array of floating-point grayscale color
 *              values that are intended to range from 0 to 1. The input specifies the size of the image, then
 *              provides none or many instructions on what to "draw". Each instruction is lead by a single
 *              character that specifies the type of instruction, followed by multiple required input values
 *              for that type of instruction and finally a color value. The color value determines which of
 *              ten possible ASCII characters the program will display at a certain point. The program prints
 *              the full image within a border of more ASCII characters as output, followed by a short
 *              description containing the minimum, maximum, and average color values within the entire image.
 */

/*
 * This function initializes the 2D array which will contain the greyscale values for the image
 *
 * Universal parameters:
 *    width -  the width of the image to draw
 *    height - the height of the image to draw
 *    image -  2D array containing the greyscale values of the image to print
 *
 * Function-specific parameters: none
 */
void initImage(int width, int height, double image[height][width]) {
    // Initialize a 2D array of the correct size, set the default greyscale value to 0.0
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = 0.0;
        }
    }
}

/*
 * This function prints out the image of ASCII characters
 *
 * Universal parameters: see initImage function
 * Function-specific parameters: none
 */
void printImage(int width, int height, double image[height][width]) {
    for (int y = -1; y <= height; y++) {
        // Print the top and bottom borders
        if (y == -1 || y == height) {
            printf("+");
            for (int x = 0; x < width; x++) {
                printf("-");
            }
            printf("+\n");
            continue;
        }

        // Print the left border
        printf("|");

        // Print the ASCII character that corresponds to the greyscale value stored in the array
        for (int x = 0; x < width; x++) {
            if (image[y][x] >= 0.0 && image[y][x] < 0.1) {
                printf(" ");
            } else if (image[y][x] >= 0.1 && image[y][x] < 0.2) {
                printf(".");
            } else if (image[y][x] >= 0.2 && image[y][x] < 0.3) {
                printf(":");
            } else if (image[y][x] >= 0.3 && image[y][x] < 0.4) {
                printf("-");
            } else if (image[y][x] >= 0.4 && image[y][x] < 0.5) {
                printf("=");
            } else if (image[y][x] >= 0.5 && image[y][x] < 0.6) {
                printf("+");
            } else if (image[y][x] >= 0.6 && image[y][x] < 0.7) {
                printf("*");
            } else if (image[y][x] >= 0.7 && image[y][x] < 0.8) {
                printf("#");
            } else if (image[y][x] >= 0.8 && image[y][x] < 0.9) {
                printf("%%");
            } else if (image[y][x] >= 0.9 && image[y][x] <= 1.0) {
                printf("@");
            } else {
                printf("INVALID COLOR");
            }
        }

        // Print the right border and a newline
        printf("|");
        printf("\n");
    }
}

/*
 * This function draws a point at the specified location with the specified color
 *
 * Universal parameters: see initImage function
 * Function-specific parameters:
 *    x -     X-coordinate of the point
 *    y -     Y-coordinate of the point
 *    color - grayscale color value to fill in at the point
 */
void drawPoint(int width, int height, double image[height][width], int x, int y, double color) {
    // Only draw a point if it is within the bounds of the image, do nothing otherwise
    if (x >= 0 && y >= 0 && x < width && y < height) {
        image[y][x] = color;
    }
}

/*
 * This function draws a filled rectangle
 *
 * Universal parameters: see initImage function
 * Function-specific parameters:
 *   x -          X-coordinate of the rectangle's top left corner
 *   y -          Y-coordinate of the rectangle's top left corner
 *   rectWidth -  the width of the rectangle
 *   rectHeight - the height of the rectangle
 *   color -      grayscale color value of the filled rectangle
 */
void drawRectangle(int width, int height, double image[height][width], int x, int y, int rectWidth, int rectHeight, double color) {
    // Call drawPoint for each (x, y) coordinate pair in the rectangle
    for (int i = y; i < y + rectHeight; i++) {
        for (int j = x; j < x + rectWidth; j++) {
            drawPoint(width, height, image, j, i, color);
        }
    }
}

/*
 * This function draws a line between two endpoints
 *
 * Universal parameters: see initImage function
 * Function-specific parameters:
 * x1 -    X-coordinate of the line's first endpoint
 * y1 -    Y-coordinate of the line's first endpoint
 * x2 -    X-coordinate of the line's second endpoint
 * y2 -    Y-coordinate of the line's secont endpoint
 * color - grayscale color value of the line
 */
void drawLine(int width, int height, double image[height][width], int x1, int y1, int x2, int y2, double color){
    // Calculate the change in x and y between the two endpoints
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Decide which coordinate plane to determine successive values along
    double step = -1.0;
    if (abs(dx) >= abs(dy)) {
        step = abs(dx);
    } else {
        step = abs(dy);
    }

    // Compute the change in x and y for each successive value in the line (aka per step)
    dx = dx / step;
    dy = dy / step;

    // Start at the first endpoint
    double x = (double) x1;
    double y = (double) y1;

    // Iterate through the line
    for (int i = 0; i <= step; i++) {
        // Draw a point on the line and move to the next successive point
        drawPoint(width, height, image, round(x), round(y), color);
        x += dx;
        y += dy;
    }
}

/*
 * This function calculates various stats regarding the total collection of color values in the image
 *
 * Universal parameters: see initImage function
 * Function-specific parameters:
 *   *min: lowest grayscale color value within the image (pointed to by min)
 *   *max: greatest grayscale color value within the image (pointed to by max) 
 *   *avg: average grayscale color value of all the characters in the image (pointed to by avg)
 */
void getImageStats(int width, int height, double image[height][width], double* min, double* max, double* avg) {
    // Set initial values for max/min
    *max = -0.1;
    *min = 1.1;
    double sumAvg = 0.0;

    // For each point in the image, update max/min if necessary then sum its color value
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (image[y][x] > *max) {
                *max = image[y][x];
            }
            if (image[y][x] < *min) {
                *min = image[y][x];
            }

            sumAvg += image[y][x];
        }
    }

    // Compute the overall average color value
    *avg = sumAvg / (width * height);
}

/*
 * This function floods the image with a specified color at a specified point
 *
 * Universal parameters: see initImage function
 * Function-specific parameters:
 * x -     X-coordinate to start flooding at
 * y -     Y-coordinate to start flooding at
 * color - grayscale color value to flood the image with
 */
void floodFill(int width, int height, double image[height][width], int x, int y, double color) {
    // Only spread flooding if the flood's color value is greater than that of the point it encounters
    // Also verify that the flooding is within the bounds of the image
    if (x >= 0 && y >= 0 && x < width && y < height) {
        if (color > image[y][x]) {
            image[y][x] = color;
        
            // Recursively flood in the four cardinal directions
            floodFill(width, height, image, x - 1, y, color);
            floodFill(width, height, image, x + 1, y, color);
            floodFill(width, height, image, x, y - 1, color);
            floodFill(width, height, image, x, y + 1, color);
        }
    }
}

// Print the resulting greyscale image as ASCII art.
// You need to fix the lines marked with TODO comments to read input from standard input.
// Do not change other things in the main function.
int main(void) {
    // Read in the size of the drawing canvas
    int width = 0;
    int height = 0;
    int result = scanf("%d %d", &width, &height);
        
    // Program only supports images that are 1x1 or bigger
    if ((width <= 0) || (height <= 0) || (result != 2)) {
        printf("Failed to read a valid width and height from standard input!\n");
        return 0;
    }
    
    // Create the 2D arary and initialize all the greyscale values to 0.0
    double image[height][width];
    initImage(width, height, image);
    
    char command = '\0';
    double color = 0.0;
    
    // Keep reading in drawing commands until we reach the end of the input
    while (scanf(" %c", &command) == 1) {
        switch (command) {		
            case 'p': {
                // Draw a point, read in: x, y, color
                int x = 0;
                int y = 0;
                result = scanf(" %d %d %lf", &x, &y, &color);
                if (result != 3) {
                    printf("Invalid point command!\n");
                    return 0;
                }
		
                drawPoint(width, height, image, x, y, color);
                break;
            }
            case 'r': {
                // Draw a rectangle, read in: x, y, w, h, color
                int left = 0;
                int top = 0;
                int rectangleWidth = 0;
                int rectangleHeight = 0;
                result = scanf(" %d %d %d %d %lf", &left, &top, &rectangleWidth, &rectangleHeight, &color);
                if (result != 5) {
                    printf("Invalid rectangle command!\n");
                    return 0;
                }
                
                drawRectangle(width, height, image, left, top, rectangleWidth, rectangleHeight, color);
                break;
            }
            case 'l': {
                // Draw a line, read in x1, y1, x2, y2, color
                int x1 = 0;
                int y1 = 0;
                int x2 = 0;
                int y2 = 0;                
                result = scanf(" %d %d %d %d %lf", &x1, &y1, &x2, &y2, &color);
                if (result != 5) {
                    printf("Invalid line command!\n");
                    return 0;
                }
                drawLine(width, height, image, x1, y1, x2, y2, color);
                break;
            }            
            case 'f': {
                // Flood fill a color in, read in: x, y, color
                int x = 0;
                int y = 0;
                result = scanf(" %d %d %lf", &x, &y, &color);
                if (result != 3) {
                    printf("Invalid flood fill command!\n");
                    return 0;
                }
                
                floodFill(width, height, image, x, y, color);
                break;
            }
            default: {
                printf("Unknown command!\n");
                return 0;
            }
        }
    }
	
    // Print the final image
    printImage(width, height, image);
    
    // Finally display some statistics about the image
    double minColor = 0.0;
    double maxColor = 0.0;
    double avgColor = 0.0;
    getImageStats(width, height, image, &minColor, &maxColor, &avgColor);
    printf("Color range [%.2f, %.2f], average %.4f\n", minColor, maxColor, avgColor);
}
