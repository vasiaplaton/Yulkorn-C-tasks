#include "stackTree.h"
#include <stdlib.h>

#define INITIAL_CAPACITY 16

StackTree createStackTree() {
    StackTree s = malloc(sizeof(_StackTree));
    s->capacity = INITIAL_CAPACITY;
    s->elems    = malloc(sizeof(STACK_TREE_T) * s->capacity);
    s->last     = 0;
    return s;
}

void destroyStackTree(StackTree s) {
    free(s->elems);
    free(s);
}

bool emptyStackTree(StackTree s) {
    return s->last == 0;
}

int depthStackTree(StackTree s) {
    return s->last;
}

void pushStackTree(StackTree s, STACK_TREE_T t) {
    if (s->last == s->capacity) {
        int newCap = s->capacity * 2;
        STACK_TREE_T *newArr = malloc(sizeof(STACK_TREE_T) * newCap);
        for (int i = 0; i < s->capacity; i++) {
            newArr[i] = s->elems[i];
        }
        free(s->elems);
        s->elems    = newArr;
        s->capacity = newCap;
    }
    s->elems[s->last++] = t;
}

STACK_TREE_T topStackTree(StackTree s) {
    if (emptyStackTree(s)) return NULL;
    return s->elems[s->last - 1];
}

void popStackTree(StackTree s) {
    if (!emptyStackTree(s)) {
        s->last--;
    }
}
