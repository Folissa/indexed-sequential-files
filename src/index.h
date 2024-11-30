#pragma once

#ifndef INDEX_H
#define INDEX_H

#include <stdlib.h>

#include "constants.h"

typedef struct {
    int data_page_index;
    int key;
} index_t;

index_t *create_index(int data_page_index, int key);

void initialize_index(index_t *index, int data_page_index, int key);

void destroy_index(index_t *index);

int index_exists(index_t *index);

void copy_index(index_t *source, index_t *destination);

#endif // INDEX_H