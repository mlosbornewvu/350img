#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iplib2New.h"

#define N 3


//Function prototypes


int apply_filter(char imagePass[][COLS], int sobel[N][N], int a, int b);
void binarize(char arrayBinary[][COLS]);




int main(int argc, char **argv)
{



    int i=0, j=0, x=0, y=0;
    int value_v = 0, value_h = 0; //Loop vars and conditions
     //File type


    char sub_block[3][3]; //sub block for image

    image_ptr imagePtr, imagePtr2; //image pointer types


    //Creating sobel filters
    int sobel_h[3][3] = {-1, -2, -1, 0, 0, 0, 1, 2, 1}; //Horizontal
    int sobel_v[3][3] = {-1, 0, 1, -2, 0, 2, -1, 0, 1}; //Vertical

    /* check inputs */
    if (argc != 8)
    {
        printf("wrong inputs: use %s infile out1 out2 \n", argv[0]);
        return 0;
    }

    /* first read-in the image */
    printf("\nReading input image... \n");

    //Pointer to access the whole image
    imagePtr = read_pnm(argv[1], &ROWS, &COLS, &TYPE); //Returns ptr from read_pnm

    printf("\nYour image was read successfully!\n");
    printf("Info about your image: rows = %d, cols = %d, type = %d\n", ROWS, COLS, TYPE);
    char imageMain[ROWS][COLS];
    char image2[ROWS][COLS];
    char image3[ROWS][COLS];
    char image4[ROWS][COLS];   /* space for output image */


    for(i=0; i<ROWS; i++){
        for(j=0; j<ROWS; j++){
            imageMain[i][j] = imagePtr[i*COLS+j];

        }

    }


    //Going over the whole image
    for(i=0; i<ROWS; i++) //The height of the image
    {
        for(j=0; j<COLS; j++) //The width of the image
        {


            value_h = apply_filter(imageMain, sobel_h,i,j);
            value_v = apply_filter(imageMain, sobel_v,i,j);

            //Update output image
            image2[i][j] = (char) value_h;
            image3[i][j] = (char) value_v;
            image4[i][j] = (char) (value_h+value_v);
        }
    }

    printf("\nPicked up a piece of image.\n");



    imagePtr2 = (image_ptr)image2;

    printf("\nNow writing to image file ... \n");
    printf("rows=%d, cols=%d, type=%d \n", ROWS, COLS, TYPE);

    write_pnm(imagePtr2, argv[2], ROWS, COLS, TYPE); //Creating the out1.pgm
    imagePtr2 = (image_ptr) image3;
    write_pnm(imagePtr2, argv[3], ROWS, COLS, TYPE); //Creating the out1.pgm
    imagePtr2 = (image_ptr) image4;
    write_pnm(imagePtr2, argv[4], ROWS, COLS, TYPE); //Creating the out1.pgm



    binarize(image2);
    imagePtr2 = (image_ptr)image2;
    write_pnm(imagePtr2, argv[5], ROWS, COLS, TYPE); //Creating the out2.pgm
    binarize(image3);
    imagePtr2 = (image_ptr)image3;
    write_pnm(imagePtr2, argv[6], ROWS, COLS, TYPE); //Creating the out2.pgm
    binarize(image4);
    imagePtr2 = (image_ptr)image4;
    write_pnm(imagePtr2, argv[7], ROWS, COLS, TYPE); //Creating the out2.pgm


    return 0;
}

int apply_filter(char imagePass[][COLS], int sobel[3][3],int a, int b)
{
    int x = 0;
    int y = 0;
    int pixel_value = 0;


    for(x = -1; x<2; x++)
    {
        for(y=-1; y<2; y++)
        {
            pixel_value += sobel[x+1][y+1]*imagePass[a+x][b+y];
        }
    }

    return pixel_value;
}




void binarize(char arrayBinary[][COLS]){
    int i =0;
    int j=0;
    int pixel_value_b = 0;
    double arrayMean=0;
    double arrayStdDev=0;
    double tempSum=0;
    double valueT=0;

    printf("%d\n %d\n", ROWS, COLS);


    for(i=0; i<ROWS; i++){
        for(j=0; j<COLS; j++){
            pixel_value_b += arrayBinary[i][j];

        }

    }
    arrayMean = (double) pixel_value_b/(ROWS * COLS);
    printf("Mean:  %f\n", arrayMean);
    for(i=0; i<ROWS; i++){
        for(j=0; j<COLS; j++){
            tempSum += pow(arrayBinary[i][j] - arrayMean, 2);

        }

    }

    arrayStdDev = sqrt((tempSum)/((ROWS*COLS)-1));
    printf("StdDev:  %f\n", arrayStdDev);

    valueT = arrayMean+arrayStdDev;

    for(i=0; i<ROWS; i++){
        for(j=0; j<COLS; j++){
            if(arrayBinary[i][j] >= valueT){
                arrayBinary[i][j]=(char) 1;
            }
            else{
                arrayBinary[i][j]=(char) 255;
            }

        }

    }


