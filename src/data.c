#include "data.h"

data_t *create_data(char *filename) {
    init_file(filename);
    data_t *data = (data_t *)malloc(sizeof(data_t));
    initialize_data(data, filename);
    data->page = create_data_page();
    return data;
}

void initialize_data(data_t *data, char *filename) {
    data->filename = filename;
    data->page_index = 0;
    data->writes = 0;
    data->reads = 0;
}

void destroy_data(data_t *data) {
    destroy_data_page(data->page);
    free(data);
}

// void handle_full_page(data_t *data, int write, int read) {
//     if (is_data_page_full(*(data->page))) {
//         if (write)
//             write_page(data);
//         data->page_index++;
//         data->page->record_index = 0;
//         if (read)
//             read_page(data);
//     }
// }

// void write_record(FILE *file, record_t *record, int record_index) {
//     int record_size = PARAMETERS_COUNT * INT_WIDTH;
//     long int record_offset = record_index * record_size;
//     if (fseek(file, record_offset, SEEK_SET) != 0) {
//         perror("Error seeking in file");
//         return;
//     }
//     fprintf(file, "%0*d%0*d%0*d",
//         INT_WIDTH, record->mass,
//         INT_WIDTH, record->specific_heat_capacity,
//         INT_WIDTH, record->temperature_change);
// }

// void write_page(data_t *data) {
//     FILE *file = open_file(data->filename, "r+");
//     for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
//         int record_index = data->page_index * RECORD_COUNT_PER_PAGE + i;
//         if (!record_exists(data->page->records[i]))
//             break;
//         write_record(file, data->page->records[i], record_index);
//         initialize_record(data->page->records[i], DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
//     }
//     close_file(file);
//     (data->writes)++;
// }

// void read_record(data_t *data, char *buffer, int record_index) {
//     int record_size = PARAMETERS_COUNT * INT_WIDTH;
//     int record_offset = record_index * record_size;
//     if (buffer[record_offset] == '\0') {
//         // Situation: there are records to read, but it will not fill the whole page,
//         // so we initalize as they do not exist
//         initialize_record(data->page->records[record_index], DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
//         return;
//     }
//     char temp[INT_WIDTH + NULL_CHARACTER_SIZE];
//     temp[INT_WIDTH] = '\0';
//     memcpy(temp, buffer + record_offset + FIRST_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
//     data->page->records[record_index]->mass = atoi(temp);
//     memcpy(temp, buffer + record_offset + SECOND_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
//     data->page->records[record_index]->specific_heat_capacity = atoi(temp);
//     memcpy(temp, buffer + record_offset + THIRD_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
//     data->page->records[record_index]->temperature_change = atoi(temp);
// }

// void read_page(data_t *data) {
//     FILE *file = open_file(data->filename, "r");
//     int page_index = data->page_index;
//     int record_size = PARAMETERS_COUNT * INT_WIDTH;
//     int records_size = RECORD_COUNT_PER_PAGE * record_size;
//     long int page_offset = page_index * records_size;
//     if (fseek(file, page_offset, SEEK_SET) != 0) {
//         perror("Error seeking in file");
//     }
//     char buffer[records_size + NULL_CHARACTER_SIZE];
//     for (int i = 0; i < records_size + NULL_CHARACTER_SIZE; i++) {
//         buffer[i] = '\0';
//     }
//     if (fread(buffer, sizeof(char), records_size, file)) {
//         for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++)
//             read_record(data, buffer, i);
//         (data->reads)++;

//     } else {
//         // Reached EOF, mark whole page as non existing
//         for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
//             initialize_record(data->page->records[i], DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
//         }
//     }
//     close_file(file);
// }

// int is_at_end(data_t *data) {
//     return !(record_exists(data->page->records[data->page->record_index]));
// }

// void add_record(data_t *data, record_t *record) {
//     if (record_exists(data->page->records[data->page->record_index])) {
//         (data->page->record_index)++;
//     }
//     handle_full_page(data, 1, 0);
//     data->page->records[data->page->record_index]->mass = record->mass;
//     data->page->records[data->page->record_index]->specific_heat_capacity = record->specific_heat_capacity;
//     data->page->records[data->page->record_index]->temperature_change = record->temperature_change;
// }

// record_t *get_next(data_t *data) {
//     data->page->record_index++;
//     handle_full_page(data, 0, 1);
//     return data->page->records[data->page->record_index];
// }

// record_t *get_current(data_t *data) {
//     return data->page->records[data->page->record_index];
// }

// void reset_data(data_t *data) {
//     reset_page(data);
//     data->page_index = 0;
// }

// void reset_page(data_t *data) {
//     data->page->record_index = 0;
//     for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
//         initialize_record(data->page->records[i], DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
//     }
// }

// void move_to_start(data_t *data) {
//     data->page->record_index = 0;
//     data->page_index = 0;
//     read_page(data);
// }

void insert_record(indexes_t *indexes, data_t *data, record_t *record) {

}

void get_record(indexes_t *indexes, data_t * data, int key) {
}