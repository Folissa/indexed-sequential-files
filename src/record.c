#include "record.h"

record_t *create_record(int key, int mass, int specific_heat_capacity, int temperature_change, int overflow_pointer) {
    record_t *record = (record_t *)malloc(sizeof(record_t));
    initialize_record(record, key, mass, specific_heat_capacity, temperature_change, overflow_pointer);
    return record;
}

void initialize_record(record_t *record, int key, int mass, int specific_heat_capacity, int temperature_change, int overflow_pointer) {
    record->key = key;
    record->mass = mass;
    record->specific_heat_capacity = specific_heat_capacity;
    record->temperature_change = temperature_change;
    record->overflow_pointer = overflow_pointer;
}

void destroy_record(record_t *record) {
    free(record);
}

int record_exists(record_t *record) {
    if (record->key == EMPTY_VALUE)
        return 0;
    return 1;
}

// void append_record(FILE *file, record_t *record) {
//     fprintf(file, "%0*d%0*d%0*d%0*d",
//         INT_WIDTH, record->key,
//         INT_WIDTH, record->mass,
//         INT_WIDTH, record->specific_heat_capacity,
//         INT_WIDTH, record->temperature_change);
// }

// int count_records(char *filename) {
//     FILE *file = open_file(filename, "r");
//     int records_count = 0;
//     char buffer[RECORD_PARAMETERS_COUNT * INT_WIDTH + NULL_CHARACTER_SIZE];
//     while (fread(buffer, sizeof(char), RECORD_PARAMETERS_COUNT * INT_WIDTH, file) == RECORD_PARAMETERS_COUNT * INT_WIDTH) {
//         records_count++;
//     }
//     fclose(file);
//     return records_count;
// }

// int is_record_empty(record_t *record) {
//     if (record->key == EMPTY_VALUE) {
//         return 1;
//     }
//     return 0;
// }

void copy_record(record_t *source, record_t *destination) {
    destination->key = source->key;
    destination->mass = source->mass;
    destination->specific_heat_capacity = source->specific_heat_capacity;
    destination->temperature_change = source->temperature_change;
    destination->overflow_pointer = source->overflow_pointer;
}

void print_record(record_t *record) {
    char format[50] = "";
    for (int i = 0; i < INT_WIDTH; i++) {
        strcat(format, "#");
    }

    if (record_exists(record)) {
        printf("KEY: %*d, MASS: %*d, SPECIFIC_HEAT_CAPACITY: %*d, TEMPERATURE_CHANGE: %*d, ",
            INT_WIDTH, record->key,
            INT_WIDTH, record->mass,
            INT_WIDTH, record->specific_heat_capacity,
            INT_WIDTH, record->temperature_change);
        if (record->overflow_pointer == EMPTY_VALUE) {
            printf("OVERFLOW_POINTER: %s\n", format);
        } else
            printf("OVERFLOW_POINTER: %*d\n", INT_WIDTH, record->overflow_pointer);
    } else
        printf("KEY: %s MASS: %s, SPECIFIC_HEAT_CAPACITY: %s, TEMPERATURE_CHANGE: %s, OVERFLOW_POINTER: %s\n",
            format,
            format,
            format,
            format,
            format);
}