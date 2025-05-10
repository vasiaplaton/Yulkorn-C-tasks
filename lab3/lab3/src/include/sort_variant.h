#pragma once

#include "udt_list.h"

// Шаг сортировки «вариантом вставки»
// Возвращает true, если узел был перемещён, false в случае уже упорядоченного списка
bool UDTList_sort_insert_variant(UDTList *lst_ptr);

// Полная сортировка списка методом вставок, переиспользует UDTList_sort_insert_variant
void UDTList_sort(UDTList *lst_ptr);