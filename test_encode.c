#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc, char **argv)
{
    // validate the argv with argc
    if(argc < 3){
        printf("Entered input not valid, please enter inputs in  the below format\n");
        printf("For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("For decoding : ./a.out -d stego.bmp output.txt\n");
        return 1;
    }


    printf("lets see what you have selected !\n");

    if(check_operation_type(argv) == e_encode){
        printf("You have seleted encode\n");
        EncodeInfo stegnography;
        if(read_and_validate_encode_args(argv, &stegnography) == e_success){
            printf("Read and validated encode arguments successfully\n");
            printf("--------------Start Encoding-----------------\n");
            if(do_encoding(&stegnography) == e_success){
                printf("Completed encoding successfully\n");
            }
            else{
                printf("Faield to perform encoding\n");
            }
        }
        else{
            printf("Failed to validate encode arguments");
            return -1;
        }
    }
    else if(check_operation_type(argv) == e_decode){
        printf("You have seleted decode");
        DecodeInfo decoInfo;
        if(read_and_validate_decode_args(argv, &decoInfo) == e_success){
            printf("Read and validated decode arguments successfully\n");
            printf("----------------Start Decoding-----------------\n");
            if(do_decoding(&decoInfo) == e_success){
                printf("Completed decoding successfully\n");
            }
            else{
                printf("Failed to perform decoding\n");
            }
            
        }
        else{
            printf("Failed to validate the decode arguements\n");
            return 1;
        }
    }
    else{
        printf("Invalid input");
        printf("Use -e for encoding\n Use -d for decoding");
    }


    return 0;
}


OperationType check_operation_type(char **argv){
    
    if(!strcmp(argv[1], "-e")){
        return e_encode;
    }
    else if(!strcmp(argv[1], "-d")){
        return e_decode;
    }
    else{
        return e_unsupported;
    }
}