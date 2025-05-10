#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "token.h"

typedef Token QUEUE_T;

typedef struct Queue {
    int first;
    int last;
    int length;
    int capacity;
    QUEUE_T *elems;
} _Queue, *Queue;

Queue createQueue();
void destroyQueue(Queue q);
bool emptyQueue(Queue q);
int lengthQueue(Queue q);
void pushQueue(Queue q, QUEUE_T t);
QUEUE_T frontQueue(Queue q);
void popQueue(Queue q);