#include "operations.h"

// Приоритеты операций: + и - — 1, * и / — 2, всё остальное — 0
int opPriority(char operation) {
        switch (operation) {
            case '+': case '-': return 1;
            case '*': case '/': return 2;
            default:            return 0;
        }
    }
    
    // Коммутативность: + и * — коммутативны, - и / — нет
bool opCommut(char operation) {
        switch (operation) {
            case '+': case '*': return true;
            case '-': case '/': return false;
            default:            return false;
        }
    }