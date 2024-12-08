#pragma once

#ifndef FILE_H
#define FILE_H

#include <stdio.h>

//  Create new file.
void create_file(const char *filename);

// Open a file in the chosen mode.
FILE *open_file(char *filename, char *mode);

//  Close a file.
void close_file(FILE *file);

// Check if file exists.
int check_file_existence(char *filename);

// Clear a file.
void clear_file(char *filename);

// Create a file if it doesn't exist or clear it if it does.
void init_file(char *filename);

// Delete a file.
void delete_file(char *filename);

// Copy contents of source file to the destination file.
void copy_file(char *source_filename, char *destination_filename);

void rename_file(const char *old_name, const char *new_name);

#endif // FILE_H