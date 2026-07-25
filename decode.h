#ifndef DECODE_H

#define DEOCODE_H

#include "types.h"
#include<stdio.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo{

    /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /*Output file Info*/
    char *output_text_fname;
    FILE *fptr_output_text;

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoInfo);

Status do_decoding(DecodeInfo *decoInfo);

Status open_files_for_decoding(DecodeInfo *decoInfo);

Status decode_magic_string(char *magicString, DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size, DecodeInfo *decoInfo);

Status decode_byte_to_lsb(char data, char *image_buffer);

#endif 
