#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[]) {

    if(argc!=2){
        fprintf(stderr, "%s\n", "Usage: source.bmp");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");

    if (!input)
    {
        strerror(errno);
        return 2;
    }



    image_t *image = malloc(sizeof(image_t));
    read_status error_code = from_bmp(input,image);
    fclose(input);

    switch(error_code) {

        case READ_OK : {
            puts("File was successfully read!");
        } break;

        case READ_INVALID_BITS : {
            fprintf(stderr, "%s\n", "Invalid bits!");
            return 4;
        }

        case READ_INVALID_HEADER : {
            fprintf(stderr, "%s\n", "Invalid file header!");
            return 5;
        }

        case READ_INVALID_SIGNATURE : {
            fprintf(stderr, "%s\n", "Invalid signature!");
            return 6;
        }
    }
    char type;
    printf("Type in which side it will be rotated (\"r\" or \"l\"): ");
    scanf("%c", &type);
    FILE *outptr = fopen(argv[1], "wb+");
    if (!outptr)
    {
        strerror(errno);
        return 3;
    }


    image_t rotated = rotate_90(*image, type);
    write_status write_status = to_bmp(outptr,&rotated);
    fclose(outptr);


    switch (write_status) {

        case WRITE_OK : {
            puts("File was successfully written!");
            if (type=='r') {
                puts("Rotated on 90 degrees right.");
            }
            else {
                puts("Rotated on 90 degrees left.");
            }
            break;
        }

        case WRITE_ERROR : {
            fprintf(stderr, "%s\n", "Error occurred while writing!");
            return 7;
        }

    }
    return 0;
}