#pragma once

#include <stdbool.h>
#include <stddef.h>   // size_t

struct list_node {
    int value;
    struct list_node *next;
};


// Синоним указателя на узел списка
typedef struct list_node* UDTList;

// Создание/удаление списка
void UDTList_create(UDTList *lst_ptr);
void UDTList_destroy(UDTList *lst_ptr);

// Основные операции
bool   UDTList_is_empty(const UDTList lst);
size_t UDTList_size(const UDTList lst);
void   UDTList_push_back(UDTList lst, int value);
int    UDTList_pop_front(UDTList lst);

// Вывод и удаление по значению
void UDTList_print(const UDTList lst);
void UDTList_erase(UDTList lst, int value);

// Копирование
void UDTList_copy(const UDTList src, UDTList dst);
