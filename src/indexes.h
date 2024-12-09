#pragma once

#ifndef INDEXES_H
#define INDEXES_H

#include <string.h>

#include "file.h"
#include "indexes_page.h"
#include "record.h"

typedef struct {
    char *filename;
    int page_index;
    indexes_page_t *page;
    int writes;
    int reads;
    int number_of_pages;
} indexes_t;

indexes_t *create_indexes(char *filename);

// Initialize all fields of the indexes.
void initialize_indexes(indexes_t *indexes, char *filename);

// Destroy a indexes.
void destroy_indexes(indexes_t *indexes);

// Handle situation when the page is full.
void handle_full_indexes_page(indexes_t *indexes, int write, int read);

// Write index to a file at specific index.
void write_index(FILE *file, index_t *index, int index_index);

// Write page to a file. Increments number of saves.
void write_indexes_page(indexes_t *indexes);

// Read index from a buffer at specific index.
void read_index(indexes_t *indexes, char *buffer, int index_index);

// Load page from a file. Increments number of loads.
void read_indexes_page(indexes_t *indexes);

// Add index to the page and handle when the page is full.
void add_index(indexes_t *indexes, index_t *index);

// Get index from the page and handle when the page is full.
index_t *get_next_index(indexes_t *indexes);

// Inserts some dummy indexes to work on.
void insert_dummy_indexes(indexes_t *indexes);

// Change values in the indexes to point to the beginning of it, and load first page.
void move_indexes_to_start(indexes_t *indexes);

// Returns the index of a page that can hold the record, based on the record's key.
index_t *find_data_page_index(indexes_t *indexes, int record_key);

// Check if there are no more indexes to read from the data.
int is_indexes_at_end(indexes_t *indexes);

#endif // INDEXES_H