#include "include/udt_list.h"
#include <stdlib.h>
#include <stdio.h>

void UDTList_create(UDTList *lst_ptr) {
    *lst_ptr = malloc(sizeof(struct list_node));
    (*lst_ptr)->next = NULL;
}

void UDTList_destroy(UDTList *lst_ptr) {
    struct list_node *curr = *lst_ptr;
    while (curr) {
        struct list_node *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    *lst_ptr = NULL;
}

bool UDTList_is_empty(const UDTList lst) {
    return lst->next == NULL;
}

size_t UDTList_size(const UDTList lst) {
    size_t cnt = 0;
    for (struct list_node *p = lst->next; p; p = p->next) cnt++;
    return cnt;
}

void UDTList_push_back(UDTList lst, int value) {
    struct list_node *p = lst;
    while (p->next) p = p->next;
    struct list_node *n = malloc(sizeof(*n));
    n->value = value;
    n->next = NULL;
    p->next = n;
}

int UDTList_pop_front(UDTList lst) {
    if (UDTList_is_empty(lst)) {
        fprintf(stderr, "Ошибка: список пуст\n");
        exit(EXIT_FAILURE);
    }
    struct list_node *f = lst->next;
    int v = f->value;
    lst->next = f->next;
    free(f);
    return v;
}

void UDTList_print(const UDTList lst) {
    for (struct list_node *p = lst->next; p; p = p->next)
        printf("%d ", p->value);
    printf("\n");
}

void UDTList_erase(UDTList lst, int value) {
    struct list_node *prev = lst, *curr = lst->next;
    while (curr) {
        if (curr->value == value) {
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void UDTList_copy(const UDTList src, UDTList dst) {
    for (struct list_node *p = src->next; p; p = p->next)
        UDTList_push_back(dst, p->value);
}
