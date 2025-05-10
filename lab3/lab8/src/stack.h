#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

typedef Token STACK_T;

typedef struct Stack {
    int last;
    int capacity;
    STACK_T *elems;
} _Stack, *Stack;

Stack createStack();
void destroyStack(Stack q);
bool emptyStack(Stack q);
int depthStack(Stack q);
void pushStack(Stack q, STACK_T t);
STACK_T topStack(Stack q);
void popStack(Stack q);