#include "operations.h"

int opPriority(char operation) {
    switch (operation) {
    case '+':
        return 1;

    case '*':
        return 2;

    // ...
    }
}

bool opCommut(char operation) {
    switch (operation) {
    case '+':
        return true;

    case '-':
        return false;

    // ...
    }
}