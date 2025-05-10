#include "../include/cli.h"
#include <stdlib.h>
#include <string.h>

void user_client() {
    deque* deq = NULL; 
    deque* copy_deq = NULL; 
    char input[256];
    
    printf("Allowed commands:\n"
           "  create                - create deque\n"
           "  destroy               - destroy deque\n"
           "  is_empty              - check if deque is empty\n"
           "  push_front <value>    - add to front\n"
           "  push_back <value>     - add to back\n"
           "  pop_front             - remove from front\n"
           "  pop_back              - remove from back\n"
           "  sort                  - sort deque\n"
           "  peek_front            - get front element\n"
           "  peek_back             - get back element\n"
           "  size                  - get deque size\n"
           "  print                 - print deque\n"
           "  load_file <file>      - load from file\n"
           "  save_to_file <file>   - save to file\n"
           "  copy_deque            - copy deque\n"
           "  exit                  - exit program\n");

    while(1) {
        printf("> ");
        fflush(stdout);
        if(!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        
        char* cmd = strtok(input, " ");
        if(!cmd) continue;
        
        if(strcmp(cmd, "exit") == 0) {
            break;
        }
        else if(strcmp(cmd, "create") == 0) {
            if(!deq) {
                deq = malloc(sizeof(deque));
                create_deque(deq);
                printf("Deque created\n");
            } else {
                printf("Deque already exists\n");
            }
        }
        else if(strcmp(cmd, "destroy") == 0) {
            if(deq) {
                destroy_deque(deq);
                free(deq);
                deq = NULL;
                printf("Deque destroyed\n");
            } else {
                printf("Deque not exists\n");
            }
        }
        else if(strcmp(cmd, "is_empty") == 0) {
            if(deq) {
                printf("Deque is empty: %d\n", deque_is_empty(deq));
            } else {
                printf("Deque not created\n");
            }
        } 
        else if(strcmp(cmd, "push_front") == 0) {
            char* arg = strtok(NULL, " ");
            if(arg && deq) {
                int value = atoi(arg);
                deque_push_front(deq, value);
                printf("Pushed front: %d\n", value);
            } else {
                printf(deq ? "Invalid argument\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "push_back") == 0) {
            char* arg = strtok(NULL, " ");
            if(arg && deq) {
                int value = atoi(arg);
                deque_push_back(deq, value);
                printf("Pushed back: %d\n", value);
            } else {
                printf(deq ? "Invalid argument\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "pop_front") == 0) {
            if(deq && !deque_is_empty(deq)) {
                printf("Popped front: %d\n", deque_pop_front(deq));
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "pop_back") == 0) {
            if(deq && !deque_is_empty(deq)) {
                printf("Popped back: %d\n", deque_pop_back(deq));
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "peek_front") == 0) {
            if(deq && !deque_is_empty(deq)) {
                printf("Front element: %d\n", deq->data[0]);
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "peek_back") == 0) {
            if(deq && !deque_is_empty(deq)) {
                printf("Back element: %d\n", deq->data[deq->size-1]);
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "size") == 0) {
            if(deq) {
                printf("Deque size: %zu\n", deque_size(deq));
            } else {
                printf("Deque not created\n");
            }
        }
        else if(strcmp(cmd, "print") == 0) {
            if(deq && !deque_is_empty(deq)) {
                deque_print(deq);
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "print") == 0) {
            if(deq && !deque_is_empty(deq)) {
                deque_print(deq);
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "load_file") == 0) {
            char* filename = strtok(NULL, " ");
            if(filename && deq) {
                bool success = read_deque_from_file(filename, deq); 
                if (success) {
                    printf("Loaded from %s\n", filename);
                }
            } else {
                printf(deq ? "Invalid filename\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "save_to_file") == 0) {
            char* filename = strtok(NULL, " ");
            if(filename && deq) {
                write_deque_to_file(filename, deq);
                printf("Saved to %s\n", filename);
            } else {
                printf(deq ? "Invalid filename\n" : "Deque not created\n");
            }
        }
        else if(strcmp(cmd, "copy_deque") == 0) {
            if(deq && !deque_is_empty(deq)) {
                if (copy_deq) {
                    destroy_deque(copy_deq);
                    free(copy_deq);
                }
                copy_deq = malloc(sizeof(deque));
                create_deque(copy_deq);
                deque_copy(deq, copy_deq);
                printf("Deque copied to copy_deq\n");
            } else {
                printf(deq ? "Deque is empty\n" : "Deque not created\n");
            }
        }
        else {
            printf("Unknown command: %s\n", cmd);
        }
    }
    
    if(deq) {
        destroy_deque(deq);
        free(deq);

        if (copy_deq) {
            destroy_deque(copy_deq);
            free(copy_deq);
        }
    }
}