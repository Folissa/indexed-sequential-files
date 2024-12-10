#pragma once

#ifndef INDEX_H
#define INDEX_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"

typedef struct {
    int data_page_index;
    int key;
} index_t;

// Create new index. Allocate memory, and initialize values.
index_t *create_index(int data_page_index, int key);

// Initialize all fields of the index.
void initialize_index(index_t *index, int data_page_index, int key);

// Destroy a index.
void destroy_index(index_t *index);

// Check for index existence.
int index_exists(index_t *index);

// Copy index values from source to destination.
void copy_index(index_t *source, index_t *destination);

void print_index(index_t *index);

#endif // INDEX_H