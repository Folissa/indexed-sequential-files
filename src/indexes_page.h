#pragma once

#ifndef INDEXES_PAGE_H
#define INDEXES_PAGE_H

#include "constants.h"
#include "index.h"

typedef struct {
    index_t *indexes[INDEXES_COUNT_PER_PAGE];
    int index_index;
} indexes_page_t;

// Create new indexes_page. Allocate memory, and initialize values.
indexes_page_t *create_indexes_page();

// Initialize all fields of the indexes_page.
void initialize_indexes_page(indexes_page_t *page);

// Destroy an indexes_page.
void destroy_indexes_page(indexes_page_t *page);

// Check if record count in indexes_page has reached INDEXES_COUNT_PER_PAGE.
int is_indexes_page_full(indexes_page_t page);

#endif // INDEXES_PAGE_H