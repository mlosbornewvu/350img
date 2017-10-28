#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iplib2New.h"

#define FILTER_SIZE 3
int ROWS, COLS, TYPE;

//Function prototypes


int apply_filter(char small_block[FILTER_SIZE][FILTER_SIZE], int sobel[FILTER_SIZE][FILTER_SIZE]);
void binarize(char arrayBinary[][COLS]);




int main(int argc, char **argv)
{


    int rows, cols, type;
    int i=0, j=0, x=0, y=0;
    int value_v = 0, value_b = 0, value_h = 0; //Loop vars and conditions
    int tp; //File type


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
    imagePtr = read_pnm(argv[1], &rows, &cols, &type); //Returns ptr from read_pnm

    printf("\nYour image was read successfully!\n");
    printf("Info about your image: rows = %d, cols = %d, type = %d\n", rows, cols, type);
    char imageMain[rows][cols];
    char image2[rows][cols];
    char image3[rows][cols];
    char image4[rows][cols];   /* space for output image */


    for(i=0; i<rows; i++){
        for(j=0; j<cols; j++){
            imageMain[i][j] = imagePtr[i*cols+j];

        }

    }


    //Going over the whole image
    for(i=1; i<rows-1; i++) //The height of the image
    {
        for(j=1; j<cols-1; j++) //The width of the image
        {

            imageMain[i][j] = imagePtr[i*cols+j];
            //Picking up our sub block of the image
            for(y=0; y<3; y++) //Width of the sub block - cols
            {
                for(x=0; x<3; x++) //Height of the sub block - rows
                {
                    sub_block[x][y]=imagePtr[(i+x)*cols+(j+y)];
                }
            }
            value_h = apply_filter(sub_block, sobel_h);
            value_v = apply_filter(sub_block, sobel_v);

            //Update output image
            image2[i][j] = (char) value_h;
            image3[i][j] = (char) value_v;
            image4[i][j] = (char) (value_h+value_v);
        }
    }

    printf("\nPicked up a piece of image.\n");

    tp = 5;



    imagePtr2 = (image_ptr)image2;

    printf("\nNow writing to image file ... \n");
    printf("rows=%d, cols=%d, type=%d \n", rows, cols, tp);

    write_pnm(imagePtr2, argv[2], rows, cols, tp); //Creating the out1.pgm
    imagePtr2 = (image_ptr) image3;
    write_pnm(imagePtr2, argv[3], rows, cols, tp); //Creating the out1.pgm
    imagePtr2 = (image_ptr) image4;
    write_pnm(imagePtr2, argv[4], rows, cols, tp); //Creating the out1.pgm



    binarize(image2);
    imagePtr2 = (image_ptr)image2;
    write_pnm(imagePtr2, argv[5], rows, cols, tp); //Creating the out2.pgm
    binarize(image3);
    imagePtr2 = (image_ptr)image3;
    write_pnm(imagePtr2, argv[6], rows, cols, tp); //Creating the out2.pgm
    binarize(image4);
    imagePtr2 = (image_ptr)image4;
    write_pnm(imagePtr2, argv[7], rows, cols, tp); //Creating the out2.pgm


    return 0;
}

int apply_filter(char small_block[][3], int sobel[3][3])
{
    int x = 0;
    int y = 0;
    int pixel_value = 0;


    for(x = 0; x<3; x++)
    {
        for(y=0; y<3; y++)
        {
            pixel_value += sobel[x][y]*small_block[x][y];
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


}
