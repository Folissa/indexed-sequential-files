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

void handle_full_data_page(data_t *data, int write, int read) {
    if (is_data_page_full(*(data->page))) {
        if (write)
            write_data_page(data);
        data->page_index++;
        data->page->record_index = 0;
        if (read)
            read_data_page(data);
    }
}

void write_record(FILE *file, record_t *record, int record_index) {
    int record_size = RECORD_PARAMETERS_COUNT * INT_WIDTH;
    long int record_offset = record_index * record_size;
    if (fseek(file, record_offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        return;
    }
    fprintf(file, "%0*d%0*d%0*d%0*d%0*d",
        INT_WIDTH, record->key,
        INT_WIDTH, record->mass,
        INT_WIDTH, record->specific_heat_capacity,
        INT_WIDTH, record->temperature_change,
        INT_WIDTH, record->overflow_pointer);
}

void write_data_page(data_t *data) {
    FILE *file = open_file(data->filename, "r+");
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        int record_index = data->page_index * RECORD_COUNT_PER_PAGE + i;
        if (!record_exists(data->page->records[i]))
            break;
        write_record(file, data->page->records[i], record_index);
        initialize_record(data->page->records[i], EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
    }
    close_file(file);
    (data->writes)++;
}

void read_record(data_t *data, char *buffer, int record_index) {
    int record_size = RECORD_PARAMETERS_COUNT * INT_WIDTH;
    int record_offset = record_index * record_size;
    if (buffer[record_offset] == '\0') {
        // Situation: there are records to read, but it will not fill the whole page,
        // so we initalize as they do not exist
        initialize_record(data->page->records[record_index], EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
        return;
    }
    char temp[INT_WIDTH + NULL_CHARACTER_SIZE];
    temp[INT_WIDTH] = '\0';
    memcpy(temp, buffer + record_offset + FIRST_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    data->page->records[record_index]->key = atoi(temp);
    memcpy(temp, buffer + record_offset + SECOND_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    data->page->records[record_index]->mass = atoi(temp);
    memcpy(temp, buffer + record_offset + THIRD_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    data->page->records[record_index]->specific_heat_capacity = atoi(temp);
    memcpy(temp, buffer + record_offset + FOURTH_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    data->page->records[record_index]->temperature_change = atoi(temp);
    memcpy(temp, buffer + record_offset + FIFTH_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    data->page->records[record_index]->overflow_pointer = atoi(temp);
}

void read_data_page(data_t *data) {
    FILE *file = open_file(data->filename, "r");
    int page_index = data->page_index;
    int record_size = RECORD_PARAMETERS_COUNT * INT_WIDTH;
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
            read_record(data, buffer, i);
        (data->reads)++;

    } else {
        // Reached EOF, mark whole page as non existing
        for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
            initialize_record(data->page->records[i], EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
        }
    }
    close_file(file);
}

int is_data_at_end(data_t *data) {
    return !(record_exists(data->page->records[data->page->record_index]));
}

void add_record(data_t *data, record_t *record) {
    if (record_exists(data->page->records[data->page->record_index])) {
        (data->page->record_index)++;
    }
    handle_full_data_page(data, 1, 0);
    copy_record(record, data->page->records[data->page->record_index]);
}

record_t *get_next_record(data_t *data) {
    data->page->record_index++;
    handle_full_data_page(data, 0, 1);
    return data->page->records[data->page->record_index];
}

record_t *get_current_record(data_t *data) {
    return data->page->records[data->page->record_index];
}

void reset_data(data_t *data) {
    reset_data_page(data);
    data->page_index = 0;
}

void reset_data_page(data_t *data) {
    data->page->record_index = 0;
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        initialize_record(data->page->records[i], EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
    }
}

void move_data_to_start(data_t *data) {
    data->page->record_index = 0;
    data->page_index = 0;
    read_data_page(data);
}

void print_data(data_t *data) {
    // Remember all the values
    int temp_page_index = data->page_index;
    int temp_record_index = data->page->record_index;
    data_page_t *temp_page = create_data_page();
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        copy_record(data->page->records[i], temp_page->records[i]);
    }
    int temp_writes = data->writes;
    int temp_reads = data->reads;
    // Print out all the records in the data
    data->page_index = 0;
    data->page->record_index = 0;
    read_data_page(data);
    record_t *record = get_current_record(data);
    int index_in_file = 0;
    int current_page_index = -1;
    if (data->filename == DATA_FILENAME)
        printf("----------------------------------------------------DATA------------------------------------------------------\n");
    else if (data->filename == OVERFLOW_FILENAME)
        printf("----------------------------------------------------OVERFLOW--------------------------------------------------\n");
    // TODO: There will be empty records in a page. So this functionality and is_data_at_end may break
    // TODO: Fix, so empty records are printed out in the specific way
    while (!is_data_at_end(data) || data->page->record_index != 0) {
        if (current_page_index != data->page_index) {
            printf("----------------------------------------------------PAGE-%02d---------------------------------------------------\n", data->page_index);
            current_page_index = data->page_index;
        }
        printf("#%02d ", index_in_file);
        print_record(record);
        index_in_file++;
        record = get_next_record(data);
    }
    // Recover all the values
    data->page_index = temp_page_index;
    data->page->record_index = temp_record_index;
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        copy_record(temp_page->records[i], data->page->records[i]);
    }
    data->writes = temp_writes;
    data->reads = temp_reads;
    destroy_data_page(temp_page);
}

void insert_dummy_data(data_t *data) {
    record_t *record1 = create_record(0, 4234, 3, 10, EMPTY_VALUE);
    record_t *record2 = create_record(450, 45, 1000, 31, 9);
    record_t *record3 = create_record(1000, 12, 2000, 3, EMPTY_VALUE);
    record_t *record4 = create_record(1200, 100, 54, 12, EMPTY_VALUE);
    record_t *record5 = create_record(2000, 76, 2000, 23, EMPTY_VALUE);

    add_record(data, record1);
    add_record(data, record2);
    add_record(data, record3);
    add_record(data, record4);
    add_record(data, record5);

    write_data_page(data);

    destroy_record(record1);
    destroy_record(record2);
    destroy_record(record3);
    destroy_record(record4);
    destroy_record(record5);
}

void insert_record(indexes_t *indexes, data_t *data, record_t *record) {
    data->page_index  = find_data_page_index(indexes, record);
    read_data_page(data);
    // TODO: Add the page to the overflow based on the recoed key if it needs so
    add_record(data, record);
    write_data_page(data);
}

void get_record(indexes_t *indexes, data_t *data, int key) {
}