#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct Priority_Items{
    __uint8_t* item;
    __u_int count;
};

struct Ruck{
    __u_int id;
    __u_int size;
    __uint8_t* compartment_1;
    __uint8_t* compartment_2;
    struct Priority_Items** pir_items_ptr;
    __u_int num_items;
};

int read_puzzle_input(struct Ruck**  r_ptr[], FILE* fp);

void init_ruck (struct Ruck** r_ptr);

int print_ruck_data (struct Ruck* r_ptr);

int compare_compartments (struct Ruck* r_ptr);

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
    struct Ruck** r_ptr = (struct Ruck **)calloc(1,sizeof(struct Ruck*)); 
    init_ruck(&r_ptr[0]);
    __u_int num_of_rucks = read_puzzle_input(&r_ptr, fp);

    for(int i =0; i < num_of_rucks; i++)
    {
        printf("========Ruck Sack %d========\n",i);
        fflush(stdout);
        compare_compartments(r_ptr[i]);
        print_ruck_data(r_ptr[i]);
    }
}

int read_puzzle_input(struct Ruck**  r_ptr[], FILE* fp){
    char* line = NULL;
    size_t len = 0;
    __ssize_t read;
    __u_int num_of_rucks = 0;

    while (-1!= (read=getline(&line, &len, fp)))
    {
        printf("%d\n",num_of_rucks);
        fflush(stdout);
        if(0!=num_of_rucks){
            *r_ptr = realloc(*r_ptr,(num_of_rucks+1)* sizeof(struct Ruck*));
            if(NULL == *r_ptr){
                printf("Error reallocating rucks array pointer\n");
                return -1;
            }
            
        }
        __u_int middle = strlen(line)/2;
        init_ruck(&(*r_ptr)[num_of_rucks]);
        
        (*r_ptr)[num_of_rucks]->id = num_of_rucks;
        (*r_ptr)[num_of_rucks]->size = strlen(line);
        (*r_ptr)[num_of_rucks]->compartment_1=calloc(middle,1);
        (*r_ptr)[num_of_rucks]->compartment_2=calloc(middle,1);
        strncpy((*r_ptr)[num_of_rucks]->compartment_1,line,middle);
        strncpy((*r_ptr)[num_of_rucks]->compartment_2,&line[middle],middle+1);

        printf("Length is is %d\n",(*r_ptr)[num_of_rucks]->size);
        printf("Compartment 1 is %s\n",(*r_ptr)[num_of_rucks]->compartment_1);
        printf("Compartment 2 is %s\n",(*r_ptr)[num_of_rucks]->compartment_2);

     
        
       num_of_rucks++;
    }

    return num_of_rucks;
}

void init_ruck (struct Ruck** r_ptr){
    (*r_ptr) = calloc(1,sizeof(struct Ruck));
    (*r_ptr)->id =0;
    (*r_ptr)->size =0;
    (*r_ptr)->compartment_1 =NULL;
    (*r_ptr)->compartment_2 =NULL;
    (*r_ptr)->pir_items_ptr =NULL;
    (*r_ptr)->num_items =0;
    
}

int compare_compartments (struct Ruck* r_ptr){
    if(NULL==r_ptr->pir_items_ptr){
        r_ptr->pir_items_ptr = calloc(1,sizeof(struct Priority_Items*)); 
        r_ptr->pir_items_ptr[0] = NULL;               
    }
    __uint8_t found = 0;
    for(int i =0; i< (r_ptr->size/2); i++){
        for(int j =0; j< (r_ptr->size/2);j++){
            if(r_ptr->compartment_1[i] == r_ptr->compartment_2[j])
            {
                if(r_ptr->num_items == 0){
                    r_ptr->pir_items_ptr[0] = calloc(1,sizeof(struct Priority_Items));
                    r_ptr->pir_items_ptr[0]->item = r_ptr->compartment_1[i];
                    r_ptr->pir_items_ptr[0]->count = 1;
                    r_ptr->num_items = 1;
                }        
                else{
                    
                    for(int k = 0; k < r_ptr->num_items; k++)
                    {
                        if(r_ptr->pir_items_ptr[k]->item == r_ptr->compartment_1[i])
                        {
                            r_ptr->pir_items_ptr[k]->count++;
                            found = 1;
                            break;
                        }
                    }
                    if(!found){
                        r_ptr->num_items++;
                        r_ptr->pir_items_ptr = realloc(r_ptr->pir_items_ptr,sizeof(struct Priority_Items*)*r_ptr->num_items);
                        r_ptr->pir_items_ptr[r_ptr->num_items-1] = calloc(1,sizeof(struct Priority_Items));
                        r_ptr->pir_items_ptr[r_ptr->num_items-1]->item = r_ptr->compartment_1[i];
                        r_ptr->pir_items_ptr[r_ptr->num_items-1]->count = 1;
                        
                    }
                    found = 0;
                }
            }
        }
    }
}

int print_ruck_data (struct Ruck* r_ptr){
    printf("Length is is %d\n",r_ptr->size);
    printf("Compartment 1 is %s\n",r_ptr->compartment_1);
    printf("Compartment 2 is %s\n",r_ptr->compartment_2);
    printf("Priority Items are the following:\n");
    fflush(stdout);
    for (int i = 0; i < r_ptr->num_items;i++){
        printf("%d.) Item = %c ; Count = %d\n", i, r_ptr->pir_items_ptr[i]->item,r_ptr->pir_items_ptr[i]->count);
    }
}