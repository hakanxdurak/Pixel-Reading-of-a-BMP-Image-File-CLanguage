#include <stdio.h>
#include <stdlib.h>

#include "header.h"

struct pixel{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

typedef struct pixel pixel;

int main(void) {

    FILE *BMP_input;
    if ((BMP_input = fopen("itu_downscaled.bmp", "rb")) == NULL) {
        printf("The file \"itu.bmp\" could not be opened\n");
    } else {
        FILE *BMP_file;
        if ((BMP_file = fopen("itu_downscaled_data.txt", "wb")) == NULL) {
            printf("The file \"itu_data.txt\" could not be created\n");
        } else {
            BITMAPFILEHEADER fileheader_input;
            fread(&fileheader_input, sizeof(BITMAPFILEHEADER), 1, BMP_input);
            BITMAPINFOHEADER infoheader_input;
            fread(&infoheader_input, sizeof(BITMAPINFOHEADER), 1, BMP_input);

            //creates 2-d array
            struct pixel **pixel_array;
            pixel_array = calloc(infoheader_input.biHeight, sizeof(pixel *));
            //pixel_array = (struct pixel **) calloc(infoheader_input.biHeight, sizeof(pixel *));
            //pixel **pixel_array = calloc(infoheader_input.biHeight, sizeof(pixel *));

            for (int i = 0; i < infoheader_input.biHeight; i++) {
                pixel_array[i] = calloc(infoheader_input.biWidth, sizeof(pixel));
                //pixel_array[i] = (struct pixel *) calloc(infoheader_input.biWidth, sizeof(pixel));
            }
            //Header editing of the text file
            fprintf(BMP_file, "%4s%1s", "", "|");
            for (int k = 1; k <= infoheader_input.biWidth; ++k) {
                fprintf(BMP_file, "%1s%7s%3d%7s%1s", "|", "", k, "", "|");
            }
            fprintf(BMP_file, "\n");
            //End of the header editing

            //all the image rows are required to start at an address which is a multiple of 4 bytes
            unsigned int long long padding = (4 - (infoheader_input.biWidth * sizeof(pixel)) % 4) % 4;

            for (int i = 0; i < infoheader_input.biHeight; i++) {
                fprintf(BMP_file, "%-4d%1s", i + 1, "|");
                for (int j = 0; j < infoheader_input.biWidth; j++) {
                    fread(&pixel_array[i][j], sizeof(pixel), 1, BMP_input);
                    fprintf(BMP_file, "| [%3d][%3d][%3d] |", pixel_array[i][j].red, pixel_array[i][j].green, pixel_array[i][j].blue);
                }
                for (int n = 0; n < padding; ++n)
                    fputc(0x00, BMP_file);
                fprintf(BMP_file, "\n");
            }
            ////////////////////////VERIFICATION////////////////////////
            //For verification, print a new bmp file to see that the image is the same as the initial
            FILE *BMP_output;
            if ((BMP_output = fopen("itu_downscaled_verification.bmp", "wb")) == NULL) {
                printf("The file \"itu_verification.bmp\" could not be created\n");
            } else {
                //save to new bmp file
                fwrite(&fileheader_input, sizeof(BITMAPFILEHEADER), 1, BMP_output);
                fwrite(&infoheader_input, sizeof(BITMAPINFOHEADER), 1, BMP_output);

                for (int i = 0; i < infoheader_input.biHeight; i++) {
                    for (int j = 0; j < infoheader_input.biWidth; j++) {
                        fwrite(&pixel_array[i][j].red, 1, 1, BMP_output);
                        fwrite(&pixel_array[i][j].green, 1, 1, BMP_output);
                        fwrite(&pixel_array[i][j].blue, 1, 1, BMP_output);
                    }
                    for (int n = 0; n < padding; ++n) {
                        fputc(0x00, BMP_output);
                    }
                }
            ////////////////////////////////////////////////////////////
                free(pixel_array);
                fclose(BMP_input);
                fclose(BMP_output);
                return 0;
            }
        }
    }
}
