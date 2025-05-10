#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#define CAPACITY 2

typedef struct {

    int* data;
    int size;
    int capacity;

} deque;

void create_deque(deque* deq);
bool deque_is_empty(deque* deq);
void deque_push_front(deque* deq, int value);
void deque_push_back(deque* deq, int value);
int deque_pop_front(deque* deq);
int deque_pop_back(deque* deq);
void deque_print(const deque* deq);
size_t deque_size(const deque* deq);
void destroy_deque(deque* deq);