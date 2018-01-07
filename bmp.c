//
// Created by Dreamer on 13/12/2017.
//

#include <malloc.h>
#include <stdlib.h>
#include "bmp.h"

read_status from_bmp(FILE* in,image_t *read) {

    bmp_header header;
    fread(&header, sizeof(header), 1, in);

    if(header.bfType!= 19778)
        return READ_INVALID_SIGNATURE;
    else if(header.biBitCount!= 24)
        return READ_INVALID_BITS;
    else if(header.bOffBits!= 54)
        return READ_INVALID_HEADER;

    read->width = header.biWidth;
    read->height = header.biHeight;
    read->data = malloc(read-> width * abs(read->height)*sizeof(pixel_t));
    int padding = (4 - (read->width * sizeof(pixel_t)) % 4) % 4;
    pixel_t *temp = read->data;

    for (int i = 0; i < abs(read->height) ; ++i) {
        for (int j = 0; j < read->width ; ++j) {
            fread(temp,sizeof(pixel_t),1,in);
            temp++;
        }

        fseek(in,padding,SEEK_CUR);
    }
    return READ_OK;
}

write_status to_bmp(FILE* out, image_t* img) {
    bmp_header header;
    header.bfType = 19778;
    header.biWidth = (uint32_t) abs(img->width);
    header.biHeight =  img->height;
    int padding = (4 - (header.biWidth * sizeof(pixel_t)) % 4) % 4;
    header.bOffBits = 54;
    header.biBitCount = 24;
    header.biSizeImage = abs(header.biHeight) * header.biWidth * sizeof(pixel_t) + abs(header.biHeight)*padding;
    header.bfileSize = header.bOffBits + header.biSizeImage;
    header.bfReserved = 0;
    header.biCompression = 0;
    header.biSize = 40;
    header.biPlanes = 1;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    header.biXPelsPerMeter = 3780;
    header.biYPelsPerMeter = 3780;
    fwrite(&header,sizeof(bmp_header),1,out);

    for (int i = 0; i < abs(header.biHeight); i++) {
        for (int j = 0; j < header.biWidth; j++) {
            pixel_t triple;
            triple = img->data[header.biWidth*i + j];
            fwrite(&triple,sizeof(pixel_t),1,out);
        }
        for (int j = 0; j < padding; j++)
        {
            fputc(0x00, out);
        }

    }
    return WRITE_OK;
}

image_t rotate_90(image_t source, char side) {
    image_t* rotated = malloc(sizeof(image_t));
    rotated->width = source.height;
    rotated->height = source.width;
    pixel_t *rotate = malloc(abs(rotated->width)*rotated->height*sizeof(pixel_t));
    for (int i = 0; i < abs(source.height); ++i) {
        for (int j = 0; j < source.width ; ++j) {
            pixel_t temp = source.data[source.width*i + j];
            if (side == 'r') {
                rotate[abs(rotated->width) * (j + 1) + i - 1] = temp;
                puts("right");
            }
            else {
                rotate[abs(rotated->width) * (j + 1) - i - 1] = temp;
                puts("left");
            }
//            rotate[abs(rotated->width) * (j + 1) - i - 1] = temp;
        }
    }
    rotated->data = rotate;
    return *rotated;
}