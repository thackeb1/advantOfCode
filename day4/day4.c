#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>

#define PAIR_DELIM ','

int read_puzzle_input (uint32_t** p_mat_ptr[], FILE* fp, uint32_t* num_of_cols, uint32_t** pair_coverage1[],uint32_t** pair_coverage2[] );
int order_coverage(uint32_t** p_mat_ptr[], uint32_t m);
int fill_elv_mat (uint32_t** p_mat_ptr[],uint32_t m,uint32_t n,uint32_t start_r,char* line, size_t len, uint32_t* min, uint32_t* max );
int print_elf_mat(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n);
int check_overlap1(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n);
int check_overlap2(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n);
int create_multi_dim_array(uint32_t** mat_ptr[],uint32_t m, uint32_t n);
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
    uint32_t** p_mat_ptr = NULL;
    uint32_t num_of_cols = 0; 
    uint32_t** pair_coverage1;
    uint32_t** pair_coverage2;
    uint32_t num_of_pairs = read_puzzle_input(&p_mat_ptr, fp,&num_of_cols, &pair_coverage1,&pair_coverage2);
    
    uint32_t** test_mat = NULL;
  
    //check_overlap1(&pair_coverage2,num_of_pairs*2,2);
    check_overlap2(&pair_coverage2,num_of_pairs*2,2);

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

int read_puzzle_input (uint32_t** p_mat_ptr[], FILE* fp, uint32_t* num_of_cols, uint32_t** pair_coverage1[],uint32_t** pair_coverage2[] ){
    char* line = NULL;
    size_t len = 0;
    size_t read;
    uint32_t num_of_pairs = 0;
    uint32_t num_of_elves = 0;
    uint32_t largest_number = 0;
    while (-1!= (read=getline(&line, &len, fp)))
    {
        uint32_t num[4]={0};
        sscanf(line,"%u%*[-]%u%*[,]%u%*[-]%u",&num[0],&num[1],&num[2],&num[3]);
        for(int i =0; i <4; i++){
            if(num[i]> largest_number){
                largest_number = num[i];
            }
        }
        num_of_pairs++;
    }
    *num_of_cols = largest_number;
    create_multi_dim_array(p_mat_ptr,num_of_pairs,largest_number);
    create_multi_dim_array(pair_coverage1,num_of_pairs,2);
    create_multi_dim_array(pair_coverage2,num_of_pairs*2,2);
    rewind(fp);
    num_of_pairs = 0;
    uint32_t min =INT32_MAX;
    uint32_t max =0;
    while (-1!= (read=getline(&line, &len, fp)))
    {
        uint32_t num[4]={0};
        /*I misread the prompt and got lazy. This should be the solution and in it's own function*/
        sscanf(line,"%u%*[-]%u%*[,]%u%*[-]%u",&num[0],&num[1],&num[2],&num[3]);
        /*I don't like this and know there is a better way but my brain is currently blanking on it*/
        (*pair_coverage2)[num_of_elves][0]= num[0];
        (*pair_coverage2)[num_of_elves][1]= num[1];
        num_of_elves++;
        (*pair_coverage2)[num_of_elves][0]= num[2];
        (*pair_coverage2)[num_of_elves][1]= num[3];
        num_of_elves++;
        
        fill_elv_mat(p_mat_ptr,2,largest_number,num_of_pairs,line,strlen(line),&min,&max);
        (*pair_coverage1)[num_of_pairs][0]=min;
        (*pair_coverage1)[num_of_pairs][1]=max;
        max =0;
        min =INT32_MAX;
        num_of_pairs++;
    }
    
    return num_of_pairs;
}

int fill_elv_mat (uint32_t** p_mat_ptr[],uint32_t m,uint32_t n,uint32_t start_r, char* line, size_t len, uint32_t* min, uint32_t* max){
    
    uint32_t num[4]={0};
    sscanf(line,"%u%*[-]%u%*[,]%u%*[-]%u",&num[0],&num[1],&num[2],&num[3]);

    for(int i =0; i <4; i++){
        
        if(num[i] < *min){
            *min =num[i];
        }
        if(num[i] > *max){
            *max = num[i];
        }
        
    }
    for(int i =num[0]; i<= num[1]; i++){
        (*p_mat_ptr)[start_r][i]=1;
    }

    for(int i =num[2]; i<= num[3]; i++){
        (*p_mat_ptr)[start_r][i]=1;
    }
    
}
int print_elf_mat(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n){
    printf("Dimensions are %dx%d\n",m,n);
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            printf(" %d,",(*p_mat_ptr)[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int check_overlap1(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n){
    uint32_t num_of_pairs = 0;
    for(int r1 =0; r1<m-1;r1+=2){
        if( ((*p_mat_ptr)[r1+1][0]>=(*p_mat_ptr)[r1][0]) && ((*p_mat_ptr)[r1+1][1]<=(*p_mat_ptr)[r1][1]) || 
        ((*p_mat_ptr)[r1][0]>=(*p_mat_ptr)[r1+1][0]) && ((*p_mat_ptr)[r1][1]<=(*p_mat_ptr)[r1+1][1])){
            num_of_pairs++;
        } 
        
    }
    printf("Number of pairs is %d\n",num_of_pairs);
}

int check_overlap2(uint32_t** p_mat_ptr[], uint32_t m, uint32_t n){
    uint32_t num_of_pairs = 0;
    uint32_t** ordered_pair;
    create_multi_dim_array(&ordered_pair,2,2);

    for(int i =0; i<m-1;i+=2){
        ordered_pair[0][0] = (*p_mat_ptr)[i][0];
        ordered_pair[0][1] = (*p_mat_ptr)[i][1];

        ordered_pair[1][0] = (*p_mat_ptr)[i+1][0];
        ordered_pair[1][1] = (*p_mat_ptr)[i+1][1];
        order_coverage(&ordered_pair,2);
       
    }

    printf("Number of pairs is %d\n",num_of_pairs);

}

int order_coverage(uint32_t** p_mat_ptr[], uint32_t m){
    uint32_t** tmp1;
    uint32_t** tmp2;
    uint32_t** tmp3;
    uint32_t t[2]={0};
    create_multi_dim_array(&tmp1,m,2);
    create_multi_dim_array(&tmp2,m,2);
    for(int r = 0; r < m; r++ ){
        tmp1[r][0] = r; 
        tmp1[r][1] = (*p_mat_ptr)[r][1] - (*p_mat_ptr)[r][0];

    }
     for(int i = 0; i < m; i++){
         for(int j = m-1; j > i; j--){
            if(tmp1[j-1][1] < tmp1[j][1]){
                t[0] = tmp1[j-1][0];
                t[1] = tmp1[j-1][1];
                tmp1[j-1][0] = tmp1[j][0];
                tmp1[j-1][1] = tmp1[j][1];
                tmp1[j][0] = t[0];
                tmp1[j][1] = t[1];
            }
        }
    }

    for(int r = 0; r < m; r++ ){
        uint32_t newIdx = tmp1[r][0];
        tmp2[r][0] = (*p_mat_ptr)[newIdx][0];
        tmp2[r][1] = (*p_mat_ptr)[newIdx][1];
      
    }
    tmp3 = (*p_mat_ptr);
    (*p_mat_ptr) = tmp2;
    free(tmp3);
}

