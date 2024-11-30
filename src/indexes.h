#pragma once

#ifndef INDEXES_H
#define INDEXES_H

#include "indexes_page.h"
#include "file.h"

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


#endif // INDEXES_H