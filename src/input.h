#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "constants.h"
#include "file.h"
#include "record.h"
#include "data.h"
#include "indexes.h"

// Print prompt string.
void print_prompt();

// Clear input buffer.
void clear_input_buffer();

// Clears input buffer based on a variable.
void clear_input_array(char *input, size_t size);

// Prints help.
void print_help();

// Print invalid input message.
void print_invalid_input_message(int invalid_format);

// Manage input from a file.
void load_operations_from_file(indexes_t *indexes, data_t *data, data_t *overflow);

// Manage user input from keyboard.
void input_operations_from_keyboard(indexes_t *indexes, data_t *data, data_t *overflow);

// Prints a menu with input options to choose from.
void print_menu();

#endif // INPUT_H