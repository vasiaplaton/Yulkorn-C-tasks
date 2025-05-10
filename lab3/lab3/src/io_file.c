#include "include/io_file.h"
#include <stdio.h>
#include <stdlib.h>

bool UDTList_read_from_file(const char *filename, UDTList lst) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Не удалось открыть файл: %s\n", filename);
        return false;
    }
    int v;
    while (fscanf(f, "%d", &v) == 1)
        UDTList_push_back(lst, v);
    if (ferror(f)) {
        printf("Ошибка при чтении файла: %s\n", filename);
        clearerr(f);
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}

void UDTList_write_to_file(const char *filename, const UDTList lst) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Не удалось открыть для записи: %s\n", filename);
        return;
}
    for (struct list_node *p = lst->next; p; p = p->next)
        fprintf(f, "%d\n", p->value);
    fclose(f);
}
