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

void handle_full_indexes_page(indexes_t *indexes, int write, int read);

void write_index(FILE *file, index_t *index, int index_index);

void write_indexes_page(indexes_t *indexes);

void read_index(indexes_t *indexes, char *buffer, int index_index);

void read_indexes_page(indexes_t *indexes);

void add_index(indexes_t *indexes, index_t *index);

index_t *get_next_index(indexes_t *indexes);

void insert_dummy_indexes(indexes_t *indexes);

void move_indexes_to_start(indexes_t *indexes);

// Returns the index of a page that can hold the record, based on the record's key.
int find_data_page_index(indexes_t *indexes, record_t *record);

int is_indexes_at_end(indexes_t *indexes);

#endif // INDEXES_H