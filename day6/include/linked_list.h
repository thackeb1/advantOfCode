#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>

typedef struct elem{
	uint8_t* data;
	struct elem* next;
}cell;

typedef cell* list;


list add_to_list(list a,uint8_t* c);
list delete_from_list(list a,uint32_t pos);
void print_list(list a);
uint32_t list_contains(list a, uint8_t* c);
uint8_t* get_item_at(list a, uint32_t i);
uint32_t list_length(list a);
