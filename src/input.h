#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "constants.h"
#include "file.h"
#include "record.h"
#include "tape.h"

// Print prompt string.
void print_prompt();

// Clear input buffer.
void clear_input_buffer();

//
void clear_input_array(char *input, size_t size);

//
void print_help();

//
void print_invalid_input_message(int invalid_format);

//
void load_operations_from_file(tape_t *indexes, tape_t *data);

//
void input_operations_from_keyboard(tape_t *indexes, tape_t *data);

// Prints a menu with input options to choose from.
void print_menu();

#endif // INPUT_H