#include "../include/linked_list.h"


list add_to_list(list a,uint8_t* c){
	
	list iter, temp;
	int i=0;
	
	if(a==NULL){
		a = (list)malloc(sizeof(cell));
		a->data = c;
		a->next = NULL;
	}
	else{
		iter = a;
		
		while(iter->next!=NULL){
			iter = iter->next;
		}
		
		temp = (list)malloc(sizeof(cell));
		temp->data = c;
		temp->next = NULL;
		
		iter->next = temp;
	}	
	return a;
}

list delete_from_list(list a,uint32_t pos){
	list itr = a;
    list prev = a; 
    uint32_t count = 0;
    if(pos ==0){
        a = a->next;
    }
    else{
        for (itr =a; itr != NULL; itr = itr->next){
            if(count == pos){
                prev->next = itr->next;
                free(itr);
            }
            prev = itr;
        }
    }
	return a;
}

void print_list(list a){
	list temp = a;
	
	printf("List contains following elements : \n");
	
	while(temp!=NULL){
		printf("%c ",temp->data);
		temp = temp->next;
	}
    printf("\n");
}

uint8_t* get_item_at(list a, uint32_t i){
    list itr;
    uint32_t count = 0;
    for (itr =a; itr != NULL; itr = itr->next){
        if(count == i){
            return itr->data;
        }
        count++;
    }
    return 0;
}

uint32_t list_contains(list a, uint8_t* c){
    list itr;
    uint32_t count =0;
    for (itr =a; itr != NULL; itr = itr->next){
        if(itr->data == c){
            return count;
        }
        count++;
    }
    return -1;
}

uint32_t list_length(list a){
    uint32_t count = 0;
    list itr;
    for (itr =a; itr != NULL; itr = itr->next){
        count++;
    }
    return count;
}