#include "../include/stack.h"



stack * new_stack(void)
/* Creates a new stack. */
 {stack *s = malloc(sizeof(stack));
  check_pointer(s);
  s->bottom = malloc(MINIMUM_SIZE * sizeof(T));
  check_pointer(s->bottom);
  s->top = s->bottom - 1;
  s->allocated_top = s->bottom + MINIMUM_SIZE - 1;
  return s;}

void destroy(stack *s)
/* Frees all the memory used for a stack. */
 {free(s->bottom);
  free(s);}

bool empty(stack *s)
/* Returns true iff there are no elements on the stack. This
is different from the stack not having enough memory reserved
for even one element, which case is never allowed to arise. */
 {return s->top < s->bottom ? true : false;}

void push(stack *s, T x)
/* Puts a new element on the stack, enlarging the latter's
memory allowance if necessary. */
 {if (s->top == s->allocated_top)
     {ptrdiff_t qtty = s->top - s->bottom + 1;
      ptrdiff_t new_qtty = GROWTH_FACTOR * qtty;
      s->bottom = realloc(s->bottom, new_qtty * sizeof(T));
      check_pointer(s->bottom);
      s->top = s->bottom + qtty - 1;
      s->allocated_top = s->bottom + new_qtty - 1;}
  *(++s->top) = x;}

T pop(stack *s)
/* Removes and returns the topmost element. The result of popping
an empty stack is undefined. */
 {return *(s->top--);}

void compress(stack *s)
/* Frees any memory the stack isn't actually using. The
allocated portion still isn't allowed to shrink smaller than
MINIMUM_SIZE. If all the stack's memory is in use, nothing
happens. */
 {if (s->top == s->allocated_top) return;
  ptrdiff_t qtty = s->top - s->bottom + 1;
  if (qtty < MINIMUM_SIZE) qtty = MINIMUM_SIZE;
  size_t new_size = qtty * sizeof(T);
  s->bottom = realloc(s->bottom, new_size);
  check_pointer(s->bottom);
  s->allocated_top = s->bottom + qtty - 1;}