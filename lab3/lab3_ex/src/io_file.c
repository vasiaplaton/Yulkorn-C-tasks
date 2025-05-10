#include "../include/io_file.h"

bool read_deque_from_file(char* filename, deque* deq) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File not found: %s\n", filename);
        return false;
    }

    int value;
    while (fscanf(file, "%d", &value) == 1) {
        deque_push_back(deq, value);
        printf("DEBUG: Pushed value %d\n", value);
    }

    if (ferror(file)) {
        printf("Error reading file: %s\n", filename);
        clearerr(file);
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}
void write_deque_to_file(char* filename, deque* deq) {
    FILE* file = fopen(filename, "w");
    if (!file) return;

    for (int i = 0; i < deq->size; i++) {
        fprintf(file, "%d", deq->data[i]);
        if (i < deq->size - 1) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}