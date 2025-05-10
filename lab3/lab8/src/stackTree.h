#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"

typedef Tree STACK_TREE_T;

typedef struct StackTree {
    int last;
    int capacity;
    STACK_TREE_T *elems;
} _StackTree, *StackTree;

StackTree createStackTree();
void destroyStackTree(StackTree q);
bool emptyStackTree(StackTree q);
int depthStackTree(StackTree q);
void pushStackTree(StackTree q, STACK_TREE_T t);
STACK_TREE_T topStackTree(StackTree q);
void popStackTree(StackTree q);