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
    if(argv[3] != NULL){
        printf("Recived the output file name as \n");

    }
    else{
        printf("output file not recieved, creating new as output.txt\n");
        decoInfo->output_text_fname = "output.txt";
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
        decode_byte_from_lsb(&data[i], decoInfo->image_data);
        fwrite(&data[i], 1, 1, decoInfo->fptr_output_text);
    }
}

Status decode_byte_from_lsb(char *data, char *image_buffer){

    *data = 0;    
    for(int i = 0 ; i < 8 ; i++){
        char bit = (image_buffer[i] & 1);
        *data = *data | (bit << (7-i));
    }
}

Status decode_integer_from_lsb(int *intFound, DecodeInfo *decoInfo){

    char str[32];
    *intFound = 0;
    
    fread(str,32,1,decoInfo->fptr_src_image);
    for(int i = 0; i < 32 ; i++){
        char bit = (str[i] & 1);
        *intFound = *intFound | (bit << (32-i));
    }
}

Status decode_file_extension_size(DecodeInfo *decoInfo){
    int extn_size;
    decode_integer_from_lsb(&extn_size, decoInfo);
    decoInfo->secret_file_extn_size = extn_size;
    printf("Secret file extension size is %d\n", extn_size);
    return e_success;
}

Status decode_file_extension(char *extention, DecodeInfo *decoInfo){

    decode_data_from_image(decoInfo->extn_secret_file, decoInfo->secret_file_extn_size,decoInfo);
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decoInfo){
    int secret_file_size = 0 ;
    printf("Secret file size is %d\n", secret_file_size);
    decode_integer_from_lsb(&secret_file_size, decoInfo);
    decoInfo->size_secret_file = secret_file_size;
    printf("Secret file size is %d\n", secret_file_size);
    return e_success;
}

Status decode_conetent_from_image(long size, DecodeInfo *decoInfo){

    char secret_file_contents[size];

    decode_data_from_image(secret_file_contents, size, decoInfo);

    return e_success;
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
        return e_failure;
    }

    
    printf("Lets decode the extension file size\n");
    if(decode_file_extension_size(decoInfo) == e_success){
        printf("Extension size decoded from the image\n");
    }
    else{
        printf("Failed to decode extension size from the image\n");
        return e_failure;
    }

    printf("decoInfo->output_text_fname : %s",decoInfo->output_text_fname);
    strcpy(decoInfo->extn_output_file, strchr(decoInfo->output_text_fname, '.'));

    printf("Decoding the secret file extension from the image\n");
    if(decode_file_extension(decoInfo->extn_output_file, decoInfo)==e_success){

        printf("Decoded the secret file extension\n");
        if(!strcmp(decoInfo->extn_output_file, decoInfo->extn_output_file)){
            printf("Output file and Secret file extensions are matching\n");

        }
        else{
            printf("Output file and Secret file extensions are not matching\n");
            printf("Creating a new output file in %s extension\n", decoInfo->extn_secret_file);
            ////
        }
    }
    else{
        printf("Decoding the file extension failed\n");
        return e_failure;
    }

    printf("Decoding the secret file size from the image\n");
    if(decode_secret_file_size(decoInfo) == e_success){
        printf("Suucessfully encoded the secret file size from the image\n");
    }
    else{
        printf("Failed to decode secret file size from the image\n");
        return e_failure;
    }


    printf("Decoding the conetent from the image file and storing in the output file");
    if(decode_conetent_from_image(decoInfo->size_secret_file, decoInfo) == e_success){
        printf("Done, decoded the contents and pasted in the output\n");
    }
    else{
        printf("Failed to decode the contents of the file\n");
        return e_failure;
    }



    return e_success;
}