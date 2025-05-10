#include "include/cli.h"
#include "include/udt_list.h"
#include "include/io_file.h"
#include "include/sort_variant.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void print_help(void) {
    printf(
        "Команды:\n"
        "  create          — создать список\n"
        "  destroy         — удалить список\n"
        "  is_empty        — проверка пустоты\n"
        "  size            — длина списка\n"
        "  push_back <v>   — добавить в конец\n"
        "  pop_front       — удалить первый\n"
        "  print           — вывести весь список\n"
        "  erase <v>       — удалить значение\n"
        "  load <file>     — загрузить из файла\n"
        "  save <file>     — сохранить в файл\n"
        "  copy            — копировать список\n"
        "  sort_step       — шаг сортировки вставкой\n"
        "  sort            — сортировка вставкой\n"
        "  help            — справка\n"
        "  exit            — выход\n"
    );
}

void user_client(void) {
    UDTList lst = NULL, lst_copy = NULL;
    char line[256];

    print_help();
    while (1) {
        printf("> ");
        if (!fgets(line, sizeof line, stdin)) break;
        line[strcspn(line, "\n")] = 0;
        char *cmd = strtok(line, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "exit") == 0) break;
        else if (strcmp(cmd, "help") == 0) print_help();
        else if (strcmp(cmd, "create") == 0) {
            if (!lst) { UDTList_create(&lst); printf("Список создан\n"); }
            else printf("Уже есть\n");
        }
        else if (strcmp(cmd, "destroy") == 0) {
            if (lst) { UDTList_destroy(&lst); lst = NULL; printf("Удалён\n"); }
            else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "is_empty") == 0) {
            if (lst) printf("Пуст: %s\n", UDTList_is_empty(lst) ? "да" : "нет");
            else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "size") == 0) {
            if (lst) printf("Длина: %zu\n", UDTList_size(lst));
            else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "push_back") == 0) {
            char *arg = strtok(NULL, " ");
            if (lst && arg) { int v = atoi(arg); UDTList_push_back(lst, v); printf("Добавлено %d\n", v); }
            else printf(lst ? "Некорректно\n" : "Нет списка\n");
        }
        else if (strcmp(cmd, "pop_front") == 0) {
            if (lst && !UDTList_is_empty(lst)) {
                int v = UDTList_pop_front(lst);
                printf("Удалён %d\n", v);
            } else printf(lst ? "Пусто\n" : "Нет списка\n");
        }
        else if (strcmp(cmd, "print") == 0) {
            if (lst) UDTList_print(lst);
            else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "erase") == 0) {
            char *arg = strtok(NULL, " ");
            if (lst && arg) { int v = atoi(arg); UDTList_erase(lst, v); printf("Удалено %d\n", v); }
            else printf(lst ? "Некорректно\n" : "Нет списка\n");
        }
        else if (strcmp(cmd, "load") == 0) {
            char *f = strtok(NULL, " ");
            printf("Загрузка из %s\n", f);
            if (lst && f) {
                if (UDTList_read_from_file(f, lst)) printf("Загружено из %s\n", f);
            } else printf(lst ? "Неверный файл\n" : "Нет списка\n");
        }
        else if (strcmp(cmd, "save") == 0) {
            char *f = strtok(NULL, " ");
            if (lst && f) { UDTList_write_to_file(f, lst); printf("Сохранено в %s\n", f); }
            else printf(lst ? "Неверный файл\n" : "Нет списка\n");
        }
        else if (strcmp(cmd, "copy") == 0) {
            if (lst) {
                if (lst_copy) { UDTList_destroy(&lst_copy); lst_copy = NULL; }
                UDTList_create(&lst_copy);
                UDTList_copy(lst, lst_copy);
                printf("Копия готова\n");
            } else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "sort_step") == 0) {
            if (lst) { UDTList_sort_insert_variant(&lst); printf("Сортировка шаг выполнен\n"); }
            else printf("Нет списка\n");
        }
        else if (strcmp(cmd, "sort") == 0) {
            if (lst) { UDTList_sort(&lst); printf("Сортировка завершена\n"); }
            else printf("Нет списка\n");
        }
        else {
            printf("Неизвестная команда: %s\n", cmd);
        }
    }

    if (lst)       UDTList_destroy(&lst);
    if (lst_copy)  UDTList_destroy(&lst_copy);
}
