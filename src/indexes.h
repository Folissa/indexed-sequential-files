#pragma once

#ifndef INDEXES_H
#define INDEXES_H

#include <string.h>

#include "file.h"
#include "indexes_page.h"

typedef struct {
    char *filename;
    int page_index;
    indexes_page_t *page;
    int writes;
    int reads;
} indexes_t;

indexes_t *create_indexes(char *filename);

// Initialize all fields of the indexes.
void initialize_indexes(indexes_t *indexes, char *filename);

// Destroy a indexes.
void destroy_indexes(indexes_t *indexes);

void handle_full_indexes_page(indexes_t *indexes, int write, int read);

void write_index(FILE *file, index_t *index, int index_index);

void write_indexes_page(indexes_t *indexes);

void read_index(indexes_t *indexes, char *buffer, int index_index);

void read_indexes_page(indexes_t *indexes);

void add_index(indexes_t *indexes, index_t *index);

index_t *get_next(indexes_t *indexes);

#endif // INDEXES_H