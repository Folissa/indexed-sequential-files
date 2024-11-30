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

// Input records from the user.
void input_records();

void load_operations_from_file();

//
void input_operations_from_keyboard();

// Prints a menu with input options to choose from.
void print_menu();

void choose_program_mode();

#endif // INPUT_H