#pragma once

#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "data_page.h"
#include "indexes.h"
#include "record.h"

typedef struct {
    char *filename;
    int page_index;
    data_page_t *page;
    int writes;
    int reads;
    int number_of_pages;
} data_t;

// Create new data.
data_t *create_data(char *filename, int number_of_pages);

// Initialize all fields of the data.
void initialize_data(data_t *data, char *filename, int number_of_pages);

// Destroy a data.
void destroy_data(data_t *data);

// Handle situation when the page is full.
int handle_full_data_page(data_t *data, int write, int read);

// Write record to a file at specific index.
void write_record(FILE *file, record_t *record, int record_index);

// Write page to a file. Increments number of saves.
void write_data_page(data_t *data);

// Read record from a buffer at specific index.
void read_record(data_t *data, char *buffer, int record_index);

// Load page from a file. Increments number of loads.
void read_data_page(data_t *data);

// Check if there are no more records to read from the data.
int is_data_at_end(data_t *data);

// Add record to the page and handle when the page is full.
int add_record(data_t *data, record_t *record);

// Get record from the page and handle when the page is full.
record_t *get_next_record(data_t *data);

// Get current record from the page.
record_t *get_current_record(data_t *data);

// Reset page values in the data including page_index.
void reset_data(data_t *data);

// Reset page values in the data except for page_index.
void reset_data_page(data_t *data);

// Change values in the data to point to the beginning of it, and load first page.
void move_data_to_start(data_t *data);

//
void insert_record(indexes_t *indexes, data_t *data, data_t *overflow, record_t *record);

//
void get_record(indexes_t *indexes, data_t *data, int key);

//
void print_data(data_t *data);

//
void insert_dummy_data(indexes_t *indexes, data_t *data, data_t *overflow);

//
void add_to_overflow(data_t *data, data_t *overflow, int parent_record_index, record_t *child);

//
int find_free_space(data_t *overflow);

//
int get_page_index(int record_pointer);

//
int get_record_index(int record_pointer);

int update_chain(data_t *overflow, int current_pointer, int record_pointer, record_t *record);

//
void get_next_in_chain(data_t *overflow, int pointer);

#endif // DATA_H
