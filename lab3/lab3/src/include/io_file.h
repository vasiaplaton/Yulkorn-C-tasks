#pragma once

#include <stdbool.h>
#include "udt_list.h"

// Чтение/запись списка в файл
bool UDTList_read_from_file(const char *filename, UDTList lst);
void UDTList_write_to_file(const char *filename, const UDTList lst);
