#include "tape.h"

tape_t *create_tape(char *filename) {
    init_file(filename);
    tape_t *tape = (tape_t *)malloc(sizeof(tape_t));
    initialize_tape(tape, filename);
    tape->page = create_page();
    return tape;
}

void initialize_tape(tape_t *tape, char *filename) {
    tape->filename = filename;
    tape->page_index = 0;
    tape->writes = 0;
    tape->reads = 0;
}

void destroy_tape(tape_t *tape) {
    destroy_page(tape->page);
    free(tape);
}

void handle_full_page(tape_t *tape, int write, int read) {
    if (is_page_full(*(tape->page))) {
        if (write)
            write_page(tape);
        tape->page_index++;
        tape->page->record_index = 0;
        if (read)
            read_page(tape);
    }
}

void write_record(FILE *file, record_t *record, int record_index) {
    int record_size = PARAMETERS_COUNT * INT_WIDTH;
    long int record_offset = record_index * record_size;
    if (fseek(file, record_offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        return;
    }
    fprintf(file, "%0*d%0*d%0*d",
        INT_WIDTH, record->mass,
        INT_WIDTH, record->specific_heat_capacity,
        INT_WIDTH, record->temperature_change);
}

void write_page(tape_t *tape) {
    FILE *file = open_file(tape->filename, "r+");
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        int record_index = tape->page_index * RECORD_COUNT_PER_PAGE + i;
        if (!record_exists(tape->page->records[i]))
            break;
        write_record(file, tape->page->records[i], record_index);
        // TODO: Fix key
        initialize_record(tape->page->records[i], 0);
    }
    close_file(file);
    (tape->writes)++;
}

void read_record(tape_t *tape, char *buffer, int record_index) {
    int record_size = PARAMETERS_COUNT * INT_WIDTH;
    int record_offset = record_index * record_size;
    if (buffer[record_offset] == '\0') {
        // Situation: there are records to read, but it will not fill the whole page,
        // so we initalize as they do not exist
        // TODO: Fix key
        initialize_record(tape->page->records[record_index], 0);
        return;
    }
    char temp[INT_WIDTH + NULL_CHARACTER_SIZE];
    temp[INT_WIDTH] = '\0';
    memcpy(temp, buffer + record_offset + FIRST_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    tape->page->records[record_index]->mass = atoi(temp);
    memcpy(temp, buffer + record_offset + SECOND_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    tape->page->records[record_index]->specific_heat_capacity = atoi(temp);
    memcpy(temp, buffer + record_offset + THIRD_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    tape->page->records[record_index]->temperature_change = atoi(temp);
}

void read_page(tape_t *tape) {
    FILE *file = open_file(tape->filename, "r");
    int page_index = tape->page_index;
    int record_size = PARAMETERS_COUNT * INT_WIDTH;
    int records_size = RECORD_COUNT_PER_PAGE * record_size;
    long int page_offset = page_index * records_size;
    if (fseek(file, page_offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
    }
    char buffer[records_size + NULL_CHARACTER_SIZE];
    for (int i = 0; i < records_size + NULL_CHARACTER_SIZE; i++) {
        buffer[i] = '\0';
    }
    if (fread(buffer, sizeof(char), records_size, file)) {
        for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++)
            read_record(tape, buffer, i);
        (tape->reads)++;

    } else {
        // Reached EOF, mark whole page as non existing
        for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
            // TODO: Fix key
            initialize_record(tape->page->records[i], 0);
        }
    }
    close_file(file);
}

int is_at_end(tape_t *tape) {
    return !(record_exists(tape->page->records[tape->page->record_index]));
}

void add_record(tape_t *tape, record_t *record) {
    if (record_exists(tape->page->records[tape->page->record_index])) {
        (tape->page->record_index)++;
    }
    handle_full_page(tape, 1, 0);
    tape->page->records[tape->page->record_index]->mass = record->mass;
    tape->page->records[tape->page->record_index]->specific_heat_capacity = record->specific_heat_capacity;
    tape->page->records[tape->page->record_index]->temperature_change = record->temperature_change;
}

record_t *get_next(tape_t *tape) {
    tape->page->record_index++;
    handle_full_page(tape, 0, 1);
    return tape->page->records[tape->page->record_index];
}

record_t *get_current(tape_t *tape) {
    return tape->page->records[tape->page->record_index];
}

void reset_tape(tape_t *tape) {
    reset_page(tape);
    tape->page_index = 0;
}

void reset_page(tape_t *tape) {
    tape->page->record_index = 0;
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        // TODO: Fix key
        initialize_record(tape->page->records[i], 0);
    }
}

void move_to_start(tape_t *tape) {
    tape->page->record_index = 0;
    tape->page_index = 0;
    read_page(tape);
}
