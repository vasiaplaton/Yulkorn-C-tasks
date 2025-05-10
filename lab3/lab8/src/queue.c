#include "queue.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

Queue createQueue() {
    Queue q = malloc(sizeof(_Queue));
    q->capacity = INITIAL_CAPACITY;
    q->elems     = malloc(sizeof(QUEUE_T) * q->capacity);
    q->first     = 0;
    q->last      = 0;
    q->length    = 0;
    return q;
}

void destroyQueue(Queue q) {
    free(q->elems);
    free(q);
}

bool emptyQueue(Queue q) {
    return q->length == 0;
}

int lengthQueue(Queue q) {
    return q->length;
}

void pushQueue(Queue q, QUEUE_T t) {
    if (q->length == q->capacity) {
        int newCap = q->capacity * 2;
        QUEUE_T *newArr = malloc(sizeof(QUEUE_T) * newCap);
        for (int i = 0; i < q->length; i++) {
            newArr[i] = q->elems[(q->first + i) % q->capacity];
        }
        free(q->elems);
        q->elems   = newArr;
        q->capacity = newCap;
        q->first    = 0;
        q->last     = q->length;
    }
    q->elems[q->last] = t;
    q->last = (q->last + 1) % q->capacity;
    q->length++;
}

QUEUE_T frontQueue(Queue q) {
    if (emptyQueue(q)) return NULL;
    return q->elems[q->first];
}

void popQueue(Queue q) {
    if (emptyQueue(q)) return;
    q->first = (q->first + 1) % q->capacity;
    q->length--;
}
