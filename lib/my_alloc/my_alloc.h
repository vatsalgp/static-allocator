#pragma once
#include <stdio.h>

void init_heap();

void* my_malloc(size_t bytes);

void my_free(void* data);