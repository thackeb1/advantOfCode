#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include<stdint.h>


#define check_pointer(p) if (!p) {puts("Out of memory."); exit(EXIT_FAILURE);}

#define MINIMUM_SIZE 1
 /* Minimal stack size (expressed in number of elements) for which
 space is allocated. It should be at least 1. */
#define GROWTH_FACTOR 2
 /* How much more memory is allocated each time a stack grows
 out of its allocated segment. */


typedef uint8_t* T;
typedef struct
 {T *bottom;
  T *top;
  T *allocated_top;} stack;

stack * new_stack(void);
void destroy(stack *s);
bool empty(stack *s);
void push(stack *s, T x);
T pop(stack *s);
void compress(stack *s);
