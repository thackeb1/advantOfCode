#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct Elf {
    __u_int uid;
    __u_int size;
    __u_int* calorie_array;
    __u_int total_calories;
}elf;

int read_puzzle_input (struct Elf** e_ptr[], FILE* fp );
int print_calories(struct Elf* e_ptr);
int elf_with_most_cals (struct Elf* e_ptr[], __u_int len);
int top_three_elf_with_most_cals (struct Elf* e_ptr[], __u_int len);
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
    struct Elf** e_ptr = (struct Elf **)malloc(sizeof(struct Elf*)); 
    e_ptr[0] = malloc(sizeof(struct Elf));
    e_ptr[0]->size = 0;
    e_ptr[0]->uid = 1;
    e_ptr[0]->calorie_array = NULL;
    e_ptr[0]->total_calories = 0;
    
    int num_of_elfs = read_puzzle_input(&e_ptr, fp);

    printf("Created %d elves\n",num_of_elfs);
    /*for (int i = 0; i<num_of_elfs;i++){
        printf("Elf %d calories\n",i);
        print_calories(e_ptr[i]);
        printf("\n");
    }*/
    elf_with_most_cals(e_ptr,num_of_elfs);
    top_three_elf_with_most_cals(e_ptr,num_of_elfs);

}
int print_calories(struct Elf* e_ptr){
    __u_int sum = 0;
    for(int i =0 ; i < e_ptr->size; i++)
        {
            sum+=e_ptr->calorie_array[i];
            printf("Calories[%d]: %d\n", i, e_ptr->calorie_array[i]);
        }
    printf("Total calories %d\n",sum);
}

int elf_with_most_cals (struct Elf* e_ptr[], __u_int len){
    __u_int highest_cal_count= 0;
    __u_int elf_idx = 0;
    for (int i = 0; i < len; i++){
        __u_int sum = 0;
        for (int j =0; j< e_ptr[i]->size; j++){
            sum+= e_ptr[i]->calorie_array[j];
        }
        if (sum > highest_cal_count){
            highest_cal_count = sum;
            elf_idx = i;
        }
    }
    printf("Elf with highest cal (%d) is %d\n",highest_cal_count,elf_idx);
}
top_three_elf_with_most_cals(struct Elf* e_ptr[], __u_int len){
    __u_int top_3_cals[3] = {0,0,0};
    __u_int top_3_eleves[3] = {-1,-1,-1};
    for (int i =0; i< len; i++){
        __u_int sum = 0;
        for (int j = 0; j< e_ptr[i]->size; j++){
            sum+= e_ptr[i]->calorie_array[j];
        }
        for(int k = 0; k <3; k++){
            if(top_3_cals[k] < sum)
            {
                top_3_cals[k] = sum;
                top_3_eleves[k] = i;
                break;
            }
        }
    }
    printf("Top 3 elves are %d, %d, %d carrying %d, %d, %d calories\n",top_3_eleves[0],top_3_eleves[1],top_3_eleves[2],top_3_cals[0],top_3_cals[1],top_3_cals[2]);
    printf("Total Calories are %d\n", top_3_cals[0]+top_3_cals[1]+top_3_cals[2]);
}
int read_puzzle_input(struct Elf** e_ptr[], FILE* fp){
    char* line = NULL;
    int num_of_elfs = 1;
    size_t len = 0;
    __ssize_t read;

    while (-1!= (read=getline(&line, &len, fp)))
    {
        
  
        if('\n'==line[0]){
           
            num_of_elfs++;
            struct Elf** tmp;
            tmp = realloc(*e_ptr, num_of_elfs*sizeof(struct Elf*));
            if(NULL == tmp){
                printf("Error reallocating elf pointer\n");
                return -1;
            }
            *e_ptr = tmp;
           
            (*e_ptr)[num_of_elfs-1] = malloc(sizeof(struct Elf));
            (*e_ptr)[num_of_elfs-1]->size = 0;
            (*e_ptr)[num_of_elfs-1]->uid = num_of_elfs;
            (*e_ptr)[num_of_elfs-1]->calorie_array = NULL;
            (*e_ptr)[num_of_elfs-1]->total_calories = 0;

        }
        else{
          
            if((*e_ptr)[num_of_elfs-1]->calorie_array == NULL){
              
                (*e_ptr)[num_of_elfs-1]->calorie_array = malloc(sizeof(__u_int));
                (*e_ptr)[num_of_elfs-1]->size++;
            }
            else{
                (*e_ptr)[num_of_elfs-1]->size++;
                __u_int* tmp = realloc((*e_ptr)[num_of_elfs-1]->calorie_array ,sizeof(__u_int) * (*e_ptr)[num_of_elfs-1]->size);
                if(NULL == tmp){
                    printf("Error reallocating calorie array pointer\n");
                    return -1;
                }
                (*e_ptr)[num_of_elfs-1]->calorie_array = tmp;
            }
            (*e_ptr)[num_of_elfs-1]->calorie_array[(*e_ptr)[num_of_elfs-1]->size-1] = atoi(line);
        }
       
     
    }
    return num_of_elfs;
}