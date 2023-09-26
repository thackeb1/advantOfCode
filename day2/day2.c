#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define ROCK_1 'A'
#define PAPER_1 'B'
#define SCISSORS_1 'C'

#define ROCK_2 'X'
#define PAPER_2 'Y'
#define SCISSORS_2 'Z'

#define ROCK_WEIGHT 1
#define PAPER_WEIGHT  2
#define SCISSORS_WEIGHT 3 


#define WIN 6
#define LOSS 0
#define DRAW 3

#define NEED_WIN 'Z'
#define NEED_LOSS 'X'
#define NEED_DRAW 'Y'

struct Round{
    __uint8_t mv1;
    __uint8_t mv2;
    __u_int rnd ;
    __uint8_t result;
    __uint8_t new_result;
};

void init_round (struct Round** r_ptr);

int read_puzzle_input (struct Round** r_ptr[], FILE* fp );

int calc_wins (struct Round* r_ptr[], __u_int len);

int rnd_res (struct Round** r_ptr);
int new_rnd_res (struct Round** r_ptr);

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
    struct Round** r_ptr = (struct Round **)malloc(sizeof(struct Round*)); 
    init_round(&r_ptr[0]);
    __u_int num_of_rnds = read_puzzle_input(&r_ptr, fp);

    calc_wins(r_ptr,num_of_rnds);

}
int read_puzzle_input (struct Round** r_ptr[], FILE* fp ){
    char* line = NULL;
    size_t len = 0;
    __ssize_t read;
    __u_int num_of_rnds = 0;

    while (-1!= (read=getline(&line, &len, fp)))
    {
 
        
        if(0==num_of_rnds){
            (*r_ptr)[num_of_rnds]->mv1=line[0];
            (*r_ptr)[num_of_rnds]->mv2=line[2];

        }
        else{
           
            struct Round** tmp = realloc(*r_ptr,(num_of_rnds+1)* sizeof(struct Round*));
            if(NULL == tmp){
                printf("Error reallocating calorie array pointer\n");
                return -1;
            }
            *r_ptr = tmp;
            init_round(&(*r_ptr)[num_of_rnds]);
            (*r_ptr)[num_of_rnds]->mv1=line[0];
            (*r_ptr)[num_of_rnds]->mv2=line[2];
            (*r_ptr)[num_of_rnds]->rnd=num_of_rnds;
            
        }
        rnd_res(&(*r_ptr)[num_of_rnds]);
        new_rnd_res(&(*r_ptr)[num_of_rnds]);
        num_of_rnds++;
    }
    return num_of_rnds;
}

int rnd_res (struct Round** r_ptr){

    switch ((*r_ptr)->mv1)
    {
    case ROCK_1:

        if ((*r_ptr)->mv2 == ROCK_2){
            (*r_ptr)->result = DRAW + ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == SCISSORS_2)
        {
            (*r_ptr)->result = LOSS+ SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->result = WIN+ PAPER_WEIGHT;
        }
        
        break;
    case SCISSORS_1:

        if ((*r_ptr)->mv2 == ROCK_2){
            (*r_ptr)->result = WIN + ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == SCISSORS_2){
            (*r_ptr)->result = DRAW + SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->result = LOSS + PAPER_WEIGHT;
        }
        break;
    case PAPER_1:

        if ((*r_ptr)->mv2 == ROCK_2){
            (*r_ptr)->result = LOSS + ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == SCISSORS_2)
        {
            (*r_ptr)->result = WIN + SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->result = DRAW + PAPER_WEIGHT;
        }
        break;
    default:
        break;
    }
}

int new_rnd_res (struct Round** r_ptr){

    switch ((*r_ptr)->mv1)
    {
    case ROCK_1:

        if ((*r_ptr)->mv2 == NEED_DRAW){
            (*r_ptr)->new_result = DRAW+ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == NEED_LOSS)
        {
            (*r_ptr)->new_result = LOSS+ SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->new_result = WIN+ PAPER_WEIGHT;
        }
        
        break;
    case SCISSORS_1:

        if ((*r_ptr)->mv2 == NEED_WIN){
            (*r_ptr)->new_result = WIN + ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == NEED_DRAW){
            (*r_ptr)->new_result = DRAW + SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->new_result = LOSS + PAPER_WEIGHT;
        }
        break;
    case PAPER_1:

        if ((*r_ptr)->mv2 == NEED_LOSS){
            (*r_ptr)->new_result = LOSS + ROCK_WEIGHT;
        }
        else if ((*r_ptr)->mv2 == NEED_WIN)
        {
            (*r_ptr)->new_result = WIN + SCISSORS_WEIGHT;
        }
        else{
            (*r_ptr)->new_result = DRAW + PAPER_WEIGHT;
        }
        break;
    default:
        break;
    }
}

int calc_wins (struct Round* r_ptr[], __u_int len){
    __u_int total_score = 0;
    __u_int new_total_score = 0;
    for (int i = 0; i< len; i++){

        total_score+= r_ptr[i]->result;
        new_total_score+=r_ptr[i]->new_result;
   
    }
    printf("Total old score are %d\n", total_score);
    printf("Total new score are %d\n", new_total_score);
}

void init_round (struct Round** r_ptr){
    (*r_ptr) = malloc(sizeof(struct Round));
    (*r_ptr)->mv1 =0;
    (*r_ptr)->mv2 =0;
    (*r_ptr)->result =0;
    (*r_ptr)->new_result =0;
    (*r_ptr)->rnd =0;
    
}