#include "../include/deque_copy.h"


void deque_copy(deque* original_deq, deque* copy_deq) {
    free(copy_deq->data);
    copy_deq->capacity = original_deq->capacity;
    copy_deq->size = original_deq->size;
    copy_deq->data = malloc(copy_deq->capacity * sizeof(int));
    memcpy(copy_deq->data, original_deq->data, copy_deq->size * sizeof(int));
}

//срздание копии дека для работы с ним