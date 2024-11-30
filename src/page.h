#pragma once

#ifndef PAGE_H
#define PAGE_H

#include <stdio.h>

#include "record.h"

typedef struct {
    record_t *records[RECORD_COUNT_PER_PAGE];
    int record_index;
} page_t;

// Create new page. Allocate memory, and initialize values.
page_t *create_page();

// Initialize all fields of the page.
void initialize_page(page_t *page);

// Destroy a page.
void destroy_page(page_t *page);

// Check if record count in page has reached RECORD_COUNT_PER_PAGE.
int is_page_full(page_t page);

#endif // PAGE_H