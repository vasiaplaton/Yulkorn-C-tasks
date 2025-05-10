#include "stack.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 16

Stack createStack() {
    Stack s = malloc(sizeof(_Stack));
    s->capacity = INITIAL_CAPACITY;
    s->elems    = malloc(sizeof(STACK_T) * s->capacity);
    s->last     = 0;
    return s;
}

void destroyStack(Stack s) {
    free(s->elems);
    free(s);
}

bool emptyStack(Stack s) {
    return s->last == 0;
}

int depthStack(Stack s) {
    return s->last;
}

void pushStack(Stack s, STACK_T t) {
    if (s->last == s->capacity) {
        int newCap = s->capacity * 2;
        STACK_T *newArr = malloc(sizeof(STACK_T) * newCap);
        for (int i = 0; i < s->capacity; i++) {
            newArr[i] = s->elems[i];
        }
        free(s->elems);
        s->elems    = newArr;
        s->capacity = newCap;
    }
    s->elems[s->last++] = t;
}

STACK_T topStack(Stack s) {
    if (emptyStack(s)) return NULL;
    return s->elems[s->last - 1];
}

void popStack(Stack s) {
    if (!emptyStack(s)) {
        s->last--;
    }
}
