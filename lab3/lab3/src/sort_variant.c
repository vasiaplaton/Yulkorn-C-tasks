#include "include/sort_variant.h"
#include <stdio.h>
bool UDTList_sort_insert_variant(UDTList *lst_ptr) {
    UDTList head = *lst_ptr;
    if (!head->next || !head->next->next) return true;

    struct list_node *prev = head->next;
    struct list_node *curr = prev->next;
    while (curr && curr->value >= prev->value) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        return false;
    }
    // удалить curr
    prev->next = curr->next;

    // найти место вставки
    struct list_node *p = head;
    while (p->next && p->next->value <= curr->value)
        p = p->next;

    curr->next = p->next;
    p->next = curr;

    return true;
}


void UDTList_sort(UDTList *lst_ptr) {
    // Повторяем шаг пока есть изменения
    while (UDTList_sort_insert_variant(lst_ptr)) {
        // Печатаем список после каждого шага
        printf("Текущий список: ");
        UDTList_print(*lst_ptr);
    }
}
