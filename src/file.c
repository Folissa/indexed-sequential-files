#include "file.h"

void create_file(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error creating the file");
    }
    close_file(file);
}

FILE *open_file(char *filename, char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    return file;
}

void close_file(FILE *file) {
    fclose(file);
}

int check_file_existence(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        close_file(file);
        return 1;
    } else {
        return 0;
    }
}

void clear_file(char *filename) {
    FILE *file = open_file(filename, "w");
    close_file(file);
}

void init_file(char *filename) {
    if (check_file_existence(filename)) {
        clear_file(filename);
    } else {
        create_file(filename);
    }
}

void delete_file(char *filename) {
    if (remove(filename) != 0)
        perror("Error deleting the file");
}

void copy_file(char *source_filename, char *destination_filename) {
    FILE *source_file = open_file(source_filename, "r");
    FILE *destination_file = open_file(destination_filename, "w");
    char character;
    while ((character = fgetc(source_file)) != EOF) {
        fputc(character, destination_file);
    }
    close_file(source_file);
    close_file(destination_file);
}

void rename_file(const char *old_name, const char *new_name) {
    if (rename(old_name, new_name) != 0) {
        perror("Error renaming file");
    }
}
