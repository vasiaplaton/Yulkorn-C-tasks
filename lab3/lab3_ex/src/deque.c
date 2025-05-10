#include "../include/deque.h"

void create_deque(deque* deq) {
    if (deq->data) {
        free(deq->data);
    }
    deq->data = malloc(CAPACITY * sizeof(int));
    deq->size = 0;
    deq->capacity = CAPACITY;
}
bool deque_is_empty(deque* deq) {

    return deq->size == 0;

};
void deque_push_front(deque* deq, int value) {
    if (deq->size >= deq->capacity) {
        deq->capacity *= 2;
        deq->data = realloc(deq->data, deq->capacity * sizeof(int));
    }
    memmove(deq->data + 1, deq->data, deq->size * sizeof(int));
    deq->data[0] = value;
    deq->size++;
}
void deque_push_back(deque* deq, int value) {
    if (deq->size >= deq->capacity) {
        deq->capacity *= 2;
        deq->data = realloc(deq->data, deq->capacity * sizeof(int));
    }
    deq->data[deq->size] = value;
    deq->size++;  
}
int deque_pop_front(deque* deq) {
    if (deque_is_empty(deq)) {
        fprintf(stderr, "Ошибка: очередь пуста!\n");
        exit(EXIT_FAILURE);
    }
    int value = deq->data[0];
    for (int i = 0; i < deq->size - 1; i++) {
        deq->data[i] = deq->data[i + 1];
    }
    deq->size--;
    return value;
}
int deque_pop_back(deque* deq) {
    
    if (deque_is_empty(deq)) {
        fprintf(stderr, "Ошибка: очередь пуста!\n");
        exit(EXIT_FAILURE);
    }
    deq->size--;
    return deq->data[deq->size];
}
void deque_print(const deque* deq) {

  for(int i = 0; i < deq->size; i++) {
    printf("%d ", deq->data[i]);
  }
  printf("\n");
};
size_t deque_size(const deque* deq) {

    return deq->size;

};
void destroy_deque(deque* deq) {
    if(deq) {
        free(deq->data);
        deq->data = NULL;
        deq->size = 0;
        deq->capacity = 0;
    }
}