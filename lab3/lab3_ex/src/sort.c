#include "../include/sort.h"

static void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
};
static int partition(int* arr, int low, int high) {
    int pivot = arr[(low + high) / 2]; 
    int i = low - 1;
    int j = high + 1;
    
    while (1) {
        do {
            i++;
        } while (arr[i] < pivot);
        
        do {
            j--;
        } while (arr[j] > pivot);
        
        if (i >= j) return j;
        
        swap(&arr[i], &arr[j]);
    }
}
static void quick_sort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi);
        quick_sort(arr, pi + 1, high);
    }
}
void deque_quick_sort(deque* deq) {
    if (deq->size <= 1) return;
    quick_sort(deq->data, 0, deq->size - 1);
}