#pragma once

#ifndef DATA_PAGE_H
#define DATA_PAGE_H

#include <stdio.h>

#include "record.h"

typedef struct {
    record_t *records[RECORD_COUNT_PER_PAGE];
    int record_index;
} data_page_t;

// Create new data_page. Allocate memory, and initialize values.
data_page_t *create_data_page();

// Initialize all fields of the data_page.
void initialize_data_page(data_page_t *page);

// Destroy a data_page.
void destroy_data_page(data_page_t *page);

// Check if record count in data_page has reached RECORD_COUNT_PER_data_page.
int is_data_page_full(data_page_t page);

#endif // DATA_PAGE_H