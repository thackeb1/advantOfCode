#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include <ctype.h>
#include "../include/stack.h"

#define SPACES_DELIM 4

struct Crate_Stack{
    stack* crates;
    uint32_t uid;
};


int print_mat(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n);
int read_puzzle_input (struct Crate_Stack** s_ptr[],uint32_t** m_ptr[], FILE* fp, uint8_t* num_of_stacks);
int crate_struct_init (struct Crate_Stack** s_ptr, uint32_t uid);
int create_multi_dim_array(uint32_t** mat_ptr[],uint32_t m, uint32_t n);
int print_stack(stack* s_ptr);
int correct_stack(stack** s_ptr);
int execute_moves(struct Crate_Stack* s_ptr[],uint32_t* m_mat_ptr[], uint32_t m, uint32_t n);
int execute_moves2(struct Crate_Stack* s_ptr[],uint32_t* m_mat_ptr[], uint32_t m, uint32_t n);
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

    struct Crate_Stack** s_ptr = NULL;
    uint32_t** m_ptr  = NULL;
    uint32_t num_of_moves = 0;
    uint8_t num_of_stacks = 0;

    num_of_moves=read_puzzle_input (&s_ptr, &m_ptr,fp,&num_of_stacks);
 
   
    
    //execute_moves(s_ptr,m_ptr,num_of_moves,3);
    execute_moves2(s_ptr,m_ptr,num_of_moves,3);
    get_tops_of_stack(s_ptr,num_of_stacks);


}

int read_puzzle_input (struct Crate_Stack** s_ptr[],uint32_t** m_ptr[], FILE* fp, uint8_t* num_of_stacks){
    char* line = NULL;
    size_t len = 0;
    size_t read;
    uint8_t parse_mode = 0;
    uint32_t num_of_moves= 0;
    while (-1!= (read=getline(&line, &len, fp)))
    {
        if(0==parse_mode){
            for(int i =0; i < strlen(line); i++){
                if(isdigit(line[i])){
                    (*num_of_stacks)++;
                }
            }
            if(0!=(*num_of_stacks)){
                parse_mode = 1;
            }
        }
        else if(1 == parse_mode){
            if(strlen(line)>2){
                num_of_moves++;
            }
        }
    }
    printf("Number of stacks is %d\n",(*num_of_stacks));
    printf("Number of moves is %d\n",num_of_moves);
    (*s_ptr) = calloc((*num_of_stacks),sizeof(struct Crate_Stack*));
    for(int i =0; i < (*num_of_stacks); i++){
        crate_struct_init(&(*s_ptr)[i],i+1);
    }
    create_multi_dim_array(m_ptr,num_of_moves,3);
    rewind(fp);
    parse_mode = 0;
    uint32_t mov_pos = 0;
    while (-1!= (read=getline(&line, &len, fp)))
    {
        if(0==parse_mode){
            uint32_t spaces = 0;
            uint32_t stack_pos = 0;
            for(int i = 0; i < strlen(line); i++){
                if(' ' == line[i]){
                    spaces++;
                    if(0==spaces % SPACES_DELIM){
                        stack_pos++;
                    }
                }
                else if ('[' == line[i]){
                    spaces =0;
                    
                }
                else if(isalpha(line[i])){
                    uint8_t * tmp = calloc(1,sizeof(uint8_t));
                    tmp = (uint8_t*) line[i]; 
                    push((*s_ptr)[stack_pos]->crates,tmp);
                    stack_pos++;
                }
                else if(isdigit(line[i])){
                    parse_mode = 1;
                }
                
            }
        }
        else if(1 == parse_mode){
            uint32_t moves[3];
            if(3==sscanf(line,"%*[a-z] %u %*[a-z] %u %*[a-z] %u",&moves[0],&moves[1],&moves[2])){
                (*m_ptr)[mov_pos][0] = moves[0];
                (*m_ptr)[mov_pos][1] = moves[1];
                (*m_ptr)[mov_pos][2] = moves[2];
                mov_pos++;
            }
        }
    }
    for(int i =0; i < *num_of_stacks; i++){
        correct_stack(&(*s_ptr)[i]->crates);
       // printf("=====CRATE %d=====\n", (*s_ptr)[i]->uid);
       //print_stack((*s_ptr)[i]->crates);
    }
    //print_mat(m_ptr,num_of_moves,3);
    return num_of_moves;
}

int crate_struct_init (struct Crate_Stack** s_ptr, uint32_t uid){
    (*s_ptr) = calloc(1, sizeof(struct Crate_Stack));
    (*s_ptr)->uid = uid;
    (*s_ptr)->crates = new_stack();
}

int create_multi_dim_array(uint32_t** mat_ptr[],uint32_t m, uint32_t n){
    /*To create a dynamic multi dimensional array you need to create a list of pointers and then
    allocate memory for each. So the first row in the matrix is pointers :(*/
    int* temp;
    (*mat_ptr) = calloc(m, sizeof(uint32_t*));
    temp = calloc(m * n, sizeof(uint32_t));
    for (int i = 0; i < m; i++) {
        (*mat_ptr)[i] = temp + (i * n);
    } 
}

int print_stack(stack* s_ptr){
    int i =0;

    while(!empty(s_ptr)){
        printf("%d.) %c\n",i,pop(s_ptr));
        i++;
    }
}
int correct_stack(stack** s_ptr){
    stack* tmp = new_stack();
    stack* tmp2 = *s_ptr;
    while(!empty(*s_ptr)){
        push(tmp,pop(*s_ptr));
    }
    *s_ptr = tmp;
    free(tmp2);
}

int print_mat(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n){
    printf("Dimensions are %dx%d\n",m,n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf(" %d,",(*p_mat_ptr)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int execute_moves(struct Crate_Stack* s_ptr[],uint32_t* m_mat_ptr[], uint32_t m, uint32_t n){
    uint8_t* tmp ;
    for(int i =0; i < m; i++){
        for (int j = 0; j<m_mat_ptr[i][0];j++){
            
            //printf("Moving %d crates from stack %d to stack %d\n",m_mat_ptr[i][0],
            //s_ptr[(m_mat_ptr[i][1]-1)]->uid,s_ptr[(m_mat_ptr[i][2]-1)]->uid );

            if(!empty(s_ptr[(m_mat_ptr[i][1]-1)]->crates)){
                tmp = pop(s_ptr[(m_mat_ptr[i][1]-1)]->crates);
                push(s_ptr[(m_mat_ptr[i][2]-1)]->crates, tmp);
            }
              
        }
        
    }
}

int execute_moves2(struct Crate_Stack* s_ptr[],uint32_t* m_mat_ptr[], uint32_t m, uint32_t n){
    struct Crate_Stack* tmp =new_stack();
    for(int i =0; i < m; i++){
        for (int j = 0; j<m_mat_ptr[i][0];j++){
            push(tmp,pop(s_ptr[(m_mat_ptr[i][1]-1)]->crates));
        }
        for (int j = 0; j<m_mat_ptr[i][0];j++){
            push(s_ptr[(m_mat_ptr[i][2]-1)]->crates, pop(tmp));
        }
        
        
    }
}



int get_tops_of_stack(struct Crate_Stack* s_ptr[], uint32_t num_of_stacks){
    for(int i =0; i < num_of_stacks; i++){
        printf("%c",pop(s_ptr[i]->crates));
    }
    printf("\n");
}