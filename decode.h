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
    char *magic_string_from_src_image;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /*Secret file Info*/
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];
    int secret_file_extn_size;
    long size_secret_file;

    /*Output file Info*/
    char *output_text_fname;
    FILE *fptr_output_text;
    int output_file_extn_size;
    char extn_output_file[MAX_FILE_SUFFIX];

} DecodeInfo;

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoInfo);

Status do_decoding(DecodeInfo *decoInfo);

Status open_files_for_decoding(DecodeInfo *decoInfo);

Status decode_magic_string(char *magicString, DecodeInfo *decInfo);

Status decode_data_from_image(char *data, int size, DecodeInfo *decoInfo);

Status decode_byte_from_lsb(char *data, char *image_buffer);

Status decode_file_extension_size(DecodeInfo *decodeInfo);

Status decode_file_extension(char *extention, DecodeInfo *decoInfo);

Status decode_secret_file_size(DecodeInfo *decoInfo);

Status decode_conetent_from_image(long size, DecodeInfo *DecodeInfo);


#endif 
