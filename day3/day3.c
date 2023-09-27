#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <stdint.h>

#define GROUP_SIZE 3

struct Priority_Items{
    uint8_t* item;
    uint32_t count;
    uint32_t priority;

};

struct Ruck{
    uint32_t id;
    uint32_t size;
    uint8_t* content;
    uint8_t* compartment_1;
    uint8_t* compartment_2;
    struct Priority_Items** pir_items_ptr;
    uint32_t num_items;
};

struct Group{
    struct Ruck** rucks;
    struct Priority_Items* badge;
    uint32_t uid ;
};

int read_puzzle_input(struct Ruck**  r_ptr[], struct Group** g_ptr[], FILE* fp);

void init_ruck (struct Ruck** r_ptr);
void init_group (struct Group** g_ptr);
int print_ruck_data (struct Ruck* r_ptr);

int compare_compartments (struct Ruck** r_ptr);

int sum_ruck_pir(struct Ruck* r_ptr[], uint32_t size);

int free_rucks(struct Ruck ** r_ptr[], uint32_t len);

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
    struct Ruck** r_ptr = NULL; 
    struct Group** g_ptr = NULL;
    uint32_t num_of_rucks = read_puzzle_input(&r_ptr,&g_ptr , fp);
    
    for(int i =0; i < num_of_rucks; i++)
    {
        compare_compartments(&r_ptr[i]);
        //print_ruck_data(r_ptr[i]);
    }
    
    uint32_t sum = 0;
    uint32_t res = 0;
    for(int i = 0; i< num_of_rucks/GROUP_SIZE; i++)
    {
        printf("\n\n");
        printf("======Looking group %d======\n",i);
        for(int j = 0; j < GROUP_SIZE; j++){
            printf("%d.)%s\n",j,(g_ptr[i]->rucks[j]->content));
        }
        res = find_badge(g_ptr[i]);
        printf("Adding %d to sum %d which is %d\n", res,sum,res+sum);
        sum+=res;
        printf("\n\n");
    }
    sum_ruck_pir(r_ptr,num_of_rucks);
    printf("Group sum is %d\n", sum);
    free_rucks(&r_ptr,num_of_rucks);
}

int read_puzzle_input(struct Ruck**  r_ptr[],struct Group** g_ptr[], FILE* fp){
    char* line = NULL;
    size_t len = 0;
    size_t read;
    uint32_t num_of_rucks = 0;

    while (-1!= (read=getline(&line, &len, fp)))
    {
            num_of_rucks++;
    }
    (*r_ptr) = calloc(num_of_rucks,sizeof(struct Ruck)); 
    (*g_ptr) = calloc(num_of_rucks/GROUP_SIZE,sizeof(struct Group));
    init_group (&((*g_ptr)[0]));
    num_of_rucks = 0;
    rewind(fp);
    uint32_t num_of_groups = 0;
    while (-1!= (read=getline(&line, &len, fp)))
    {
        
        uint32_t middle = strlen(line)/2;
        init_ruck(&((*r_ptr)[num_of_rucks]));
        
        (*r_ptr)[num_of_rucks]->id = num_of_rucks;
        (*r_ptr)[num_of_rucks]->size = strlen(line);
        (*r_ptr)[num_of_rucks]->content=calloc(strlen(line),1);
        (*r_ptr)[num_of_rucks]->compartment_1=calloc(middle,1);
        (*r_ptr)[num_of_rucks]->compartment_2=calloc(middle,1);
        strncpy((*r_ptr)[num_of_rucks]->content,line,strlen(line));
        strncpy((*r_ptr)[num_of_rucks]->compartment_1,line,middle);
        strncpy((*r_ptr)[num_of_rucks]->compartment_2,&line[middle],middle+1); 


        (*g_ptr)[num_of_groups]->rucks[num_of_rucks%GROUP_SIZE] =  (*r_ptr)[num_of_rucks] ;
        num_of_rucks++;
        if(num_of_rucks%GROUP_SIZE == 0 && num_of_rucks != 0){
            num_of_groups++;
            init_group (&((*g_ptr)[num_of_groups]));
        }
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

void init_group (struct Group** g_ptr){
    (*g_ptr) = calloc(1,sizeof(struct Group));
    (*g_ptr)->badge =NULL;
    (*g_ptr)->uid = 0;
    (*g_ptr)->rucks = calloc(3,sizeof(struct Ruck));
    
}

int compare_compartments (struct Ruck** r_ptr){
    if(NULL==(*r_ptr)->pir_items_ptr){
        (*r_ptr)->pir_items_ptr = calloc(1,sizeof(struct Priority_Items*)); 
        (*r_ptr)->pir_items_ptr[0] = NULL;               
    }
    uint8_t found = 0;

    for(int i =0; i< ((*r_ptr)->size/2); i++){
        for(int j =0; j< ((*r_ptr)->size/2);j++){
            if((*r_ptr)->compartment_1[i] == (*r_ptr)->compartment_2[j])
            {
                if((*r_ptr)->num_items == 0){
                    (*r_ptr)->pir_items_ptr[0] = calloc(1,sizeof(struct Priority_Items));
                    (*r_ptr)->pir_items_ptr[0]->item = (*r_ptr)->compartment_1[i];
                    (*r_ptr)->pir_items_ptr[0]->count = 1;
                    (*r_ptr)->num_items = 1;
                     if( ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item) >= 97 &&  ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1])->item<=122){
                            (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->priority = (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item-96;
                        }
                        else if( ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item)>=65 &&  ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1])->item <=90){
                            (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->priority = (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item-38;
                        }
                }        
                else{
                    
                    for(int k = 0; k < (*r_ptr)->num_items; k++)
                    {
                        if((*r_ptr)->pir_items_ptr[k]->item == (*r_ptr)->compartment_1[i])
                        {
                            (*r_ptr)->pir_items_ptr[k]->count++;
                            found = 1;
                            break;
                        }
                    }
                    if(!found){
                
                        (*r_ptr)->num_items++;
                        (*r_ptr)->pir_items_ptr = realloc((*r_ptr)->pir_items_ptr,sizeof(struct Priority_Items*)*(*r_ptr)->num_items);
                        (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1] = calloc(1,sizeof(struct Priority_Items));
                        (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item = (*r_ptr)->compartment_1[i];
                        (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->count = 1;
                        if( ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item) >= 97 &&  ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1])->item<=122){
                    
                            (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->priority = (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item-96;
                        }
                        else if( ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item)>=65 &&  ((*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1])->item <=90){
                        
                            (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->priority = (*r_ptr)->pir_items_ptr[(*r_ptr)->num_items-1]->item-38;
                        }
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
        printf("%d.) Item = %c ; Count = %d; Priority = %d\n", i, r_ptr->pir_items_ptr[i]->item,r_ptr->pir_items_ptr[i]->count,r_ptr->pir_items_ptr[i]->priority);
    }
}

int free_rucks(struct Ruck ** r_ptr[], uint32_t len){
        for(int i =0; i < len; i++){
            for(int j =0; j< (*r_ptr)[i]->num_items;j++){
                free((*r_ptr)[i]->pir_items_ptr[j]);
            }
            free((*r_ptr)[i]->pir_items_ptr);
            free((*r_ptr)[i]->compartment_1);
            free((*r_ptr)[i]->compartment_2);
            
        }
}

int sum_ruck_pir(struct Ruck* r_ptr[], uint32_t size){
    uint32_t sum = 0;
    for(int i =0; i < size; i++){
            for(int j =0; j< r_ptr[i]->num_items;j++){
                sum+= r_ptr[i]->pir_items_ptr[j]->priority;
            }
    }
    printf("The sum priority is %d\n", sum);
}

int find_badge(struct Group* g_ptr){
    uint8_t find_me = NULL;
    uint8_t found = 0;

    uint32_t num_found = 0;
  for (int i = 0; i < g_ptr->rucks[0]->size; i ++){
    find_me = g_ptr->rucks[0]->content[i];
    found=0;
    //printf("Searching for %c\n",find_me);
    for (int j = 1; j<GROUP_SIZE;j++){
       // printf("Searching Ruck %d\n",j);
        for (int k = 0; k< g_ptr->rucks[j]->size;k++){
            if(find_me == g_ptr->rucks[j]->content[k]){
                found++;
                break;
            }
        }
        if(!found){
            break;
        }
        else{
            continue;
        }
    }
    if(found == 2){
       // printf("Found %c in both ruck, done searching\n\n",find_me);
        break;
        
    }
  }
  if(NULL!= find_me){
  
   if( find_me >= 97 &&  find_me<=122){
        printf("Found %c, priority %d\n", find_me, find_me-96);     
        return  find_me-96;
    }
    else if(find_me>=65 &&  find_me <=90){
            
        printf("Found %c, priority %d\n", find_me, find_me-38);
        return  find_me-38;
    }
  }
  
}