#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include <ctype.h>
#include"../include/linked_list.h"

#define MARKER_LENGTH 14


int find_marker(FILE* fp);

int main(int argc, char** argv){
    if(2 > argc ){
        printf("Need puzzle input\n");
        return 1;        
    }
    FILE* fp;

    fp = fopen(argv[1], "r");
    if (NULL == fp){
        printf("Error opening file\n");
        return 1; 
    }
    find_marker(fp);
}

int find_marker(FILE* fp){
    uint8_t marker_found = 0;
    uint32_t stream_length = 0;
    uint8_t* curr_char;
    list marker =NULL;
    fseek(fp,0,SEEK_END);
    stream_length = ftell(fp);
    rewind(fp);
    for(int i =0; i< stream_length; i++){
        curr_char = fgetc(fp);
        if(marker == NULL){
            marker = add_to_list(marker, curr_char);
        }
        else{
            print_list(marker);
            uint32_t res = -1;
            while((res=list_contains(marker,curr_char))!=-1)
            {
                for (int j = 0; j <= res; j++){
                    marker=delete_from_list(marker,0);
                    
                }
            }
            marker = add_to_list(marker, curr_char);
            
            if(list_length(marker)==MARKER_LENGTH){
                printf("Marker Found after %d chars\n", i+1);
                break;
            }
          
            
        }
    }
    
}
