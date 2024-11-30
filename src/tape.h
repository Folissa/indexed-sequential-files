#pragma once

#ifndef TAPE_H
#define TAPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "page.h"
#include "record.h"

typedef struct {
    char *filename;
    int page_index;
    page_t *page;
    int writes;
    int reads;
} tape_t;

// Create new tape.
tape_t *create_tape(char *filename);

// Initialize all fields of the tape.
void initialize_tape(tape_t *tape, char *filename);

// Destroy a tape.
void destroy_tape(tape_t *tape);

// Handle situation when the page is full.
void handle_full_page(tape_t *tape, int write, int read);

// Write record to a file at specific index.
void write_record(FILE *file, record_t *record, int record_index);

// Write page to a file. Increments number of saves.
void write_page(tape_t *tape);

// Read record from a buffer at specific index.
void read_record(tape_t *tape, char *buffer, int record_index);

// Load page from a file. Increments number of loads.
void read_page(tape_t *tape);

// Check if there are no more records to read from the tape.
int is_at_end(tape_t *tape);

// Add record to the page and handle when the page is full.
void add_record(tape_t *tape, record_t *record);

// Get record from the page and handle when the page is full.
record_t *get_next(tape_t *tape);

// Get current record from the page.
record_t *get_current(tape_t *tape);

// Reset page values in the tape including page_index.
void reset_tape(tape_t *tape);

// Reset page values in the tape except for page_index.
void reset_page(tape_t *tape);

// Change values in the tape to point to the beginning of it, and load first page.
void move_to_start(tape_t *tape);

#ifdef DEBUG
// Print contents of a tape to stdout.
void print_tape(tape_t *tape);
#endif // DEBUG

#endif // TAPE_H
