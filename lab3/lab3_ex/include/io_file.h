#pragma once
#include"deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool read_deque_from_file(char* filename, deque* deq);
void write_deque_to_file(char* filename, deque* deq);