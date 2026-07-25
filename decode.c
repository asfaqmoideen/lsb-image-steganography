#include <stdio.h>
#include "types.h"
#include <string.h>
#include "common.h"
#include "decode.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decoInfo){

    if(strcmp(strchr(argv[2], '.'), ".bmp") == 0){
        printf("Recieved Source Image\n");
        decoInfo->src_image_fname = argv[2];
    }
    else{
        printf("Failed to get source image in .bmp format\n");
        return e_failure;
    }

    if(strcmp(strchr(argv[3], '.'), ".txt") == 0){
        printf("recieved secret message\n");
        decoInfo->output_text_fname = argv[3];
    }
    else{
        printf("Failed to recived ouput file \n");
        return e_failure;
    }

    return e_success;
}

Status open_files_for_decoding(DecodeInfo *decoInfo){

    decoInfo->fptr_src_image = fopen(decoInfo->src_image_fname, "r");

    if(decoInfo->src_image_fname == NULL){
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decoInfo->src_image_fname);

    	return e_failure;
    }

    decoInfo->fptr_output_text = fopen(decoInfo->output_text_fname, "w");
    if(decoInfo->src_image_fname == NULL){
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decoInfo->output_text_fname);

    	return e_failure;
    }

    return e_success;

}



Status decode_magic_string(char *magicString, DecodeInfo *decInfo){

    char magicStringFromImage[2];
    decode_data_from_image(magicStringFromImage, 2, decInfo);
    if(strcmp(magicStringFromImage, magicString)){
       printf("Magic string not found in the image\n");
       return e_failure; 
    }
    else{
        printf("Magic string is found in the image, decoding is possible\n");
    }
    return e_success;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decoInfo){

    for(int i = 0; i < size; i++){
        fread(decoInfo->image_data, 8, 1, decoInfo->fptr_src_image);
        decode_byte_to_lsb(data[i], decoInfo->image_data);
        fwrite(decoInfo->image_data, 8, 1, decoInfo->fptr_output_text);
    }
}

Status decode_byte_to_lsb(char data, char *image_buffer){

    for(int i = 0 ; i < 8 ; i++){
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i) & 1));
    }
}


Status do_decoding(DecodeInfo *decoInfo){

    printf("Lets open the file for decoding\n");
    if(open_files_for_decoding(decoInfo) == e_success){
        printf("Opened the file successfully with required mode\n");
    }
    else{
        printf("Failed to open the file for decoding\n");
        return e_failure;
    }

    fseek(decoInfo->fptr_src_image, 54, SEEK_SET);

    printf("Check whether the given image is encrypted using Magic string\n");
    if(decode_magic_string(MAGIC_STRING, decoInfo) == e_success){
        printf("The given image is encrypted, and ready to decrypt\n");
    }
    else{
        printf("Failed to get the Magic string from the image\n");
    }

    return e_success;
}