#include "data.h"

data_t *create_data(char *filename, int number_of_pages) {
    init_file(filename);
    data_t *data = (data_t *)malloc(sizeof(data_t));
    initialize_data(data, filename, number_of_pages);
    data->page = create_data_page();
    return data;
}

void initialize_data(data_t *data, char *filename, int number_of_pages) {
    data->filename = filename;
    data->page_index = 0;
    data->writes = 0;
    data->reads = 0;
    data->number_of_records = 0;
    data->number_of_pages = number_of_pages;
    data->last_written_page_index = EMPTY_VALUE;
    data->free_index = 0;
}

void destroy_data(data_t *data) {
    destroy_data_page(data->page);
    free(data);
}

int handle_full_data_page(data_t *data, int write, int read) {
    int page_was_written = 0;
    if (is_data_page_full(*(data->page))) {
        if (write)
            write_data_page(data);
        page_was_written = 1;
        data->page_index++;
        data->page->record_index = 0;
        if (read)
            read_data_page(data);
    }
    return page_was_written;
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
    // TODO: This guard should be implemented on some earlier stage, not when we save the page
    if (data->page_index < data->number_of_pages) {
        FILE *file = open_file(data->filename, "r+");
        for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
            int record_index = data->page_index * RECORD_COUNT_PER_PAGE + i;
            write_record(file, data->page->records[i], record_index);
            initialize_record(data->page->records[i], EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
        }
        close_file(file);
        (data->writes)++;
    }
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
    return data->page_index == data->number_of_pages && data->page->record_index == 0;
}

// TODO: Cap this, so it doesn't add more if we are out of pages
int add_record(data_t *data, record_t *record) {
    if (record_exists(data->page->records[data->page->record_index])) {
        (data->page->record_index)++;
    }
    int page_was_written = handle_full_data_page(data, 1, 0);
    copy_record(record, data->page->records[data->page->record_index]);
    return page_was_written;
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
    if (strcmp(data->filename, DATA_FILENAME) == 0)
        printf("--------------------------------------------------DATA--------------------------------------------------------\n");
    else if (strcmp(data->filename, OVERFLOW_FILENAME) == 0)
        printf("--------------------------------------------------OVERFLOW----------------------------------------------------\n");
    while (!is_data_at_end(data)) {
        if (current_page_index != data->page_index) {
            printf("--------------------------------------------------PAGE-%02d-----------------------------------------------------\n", data->page_index);
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

void insert_dummy_data(indexes_t *indexes, data_t *data, data_t *overflow) {
    record_t *record;
    record = create_record(450, 45, 1000, 31, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(600, 45, 1000, 31, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(700, 45, 1000, 31, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(800, 45, 1000, 31, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(1000, 12, 2000, 3, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(750, 4234, 3, 10, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(1200, 100, 54, 12, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);

    record = create_record(2000, 76, 2000, 23, EMPTY_VALUE);
    insert_record(indexes, data, overflow, record);
    destroy_record(record);
}

int insert_record(indexes_t *indexes, data_t *data, data_t *overflow, record_t *record) {
    // TODO: What if index starts at 2000, but we first insert 2001 and then 2000 data record?
    index_t *index = find_data_page_index(indexes, record->key);
    data->page_index = index->data_page_index;
    data->page->record_index = 0;
    read_data_page(data);
    int record_index = 0;
    record_t *previous_record = create_record(EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
    record_t *current_record = get_current_record(data);
    if (current_record->key > record->key){
        index->key = record->key;
        // TODO: Fix
        int index_index = get_record_index(index->data_page_index);
        //int index_index = get_page_index(index->data_page_index);
        copy_index(index, indexes->page->indexes[index_index]);
        write_indexes_page(indexes);
        record_t temp;
        copy_record(record, &temp);
        copy_record(current_record, record);
        copy_record(&temp, current_record);
        add_to_overflow(data, overflow, data->page->record_index, record);
        if (RECORD_COUNT_PER_PAGE * overflow->number_of_pages == overflow->number_of_records)
            reorganise(indexes, data, overflow, ALPHA);
        destroy_record(previous_record);
        destroy_index(index);
        return 1;
    }
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        if (current_record->key == record->key) {
            printf("WARNING: Record with key %d already exists!\n", current_record->key);
            destroy_record(previous_record);
            destroy_index(index);
            return 0;
        }
        // Add record when there is an empty space
        if (!(record_exists(current_record))) {
            data->page->record_index = record_index;
            int page_was_written = add_record(data, record);
            if (!page_was_written)
                write_data_page(data);
            (data->number_of_records)++;
            break;
        }
        // Parent is the previous record
        else if ((record_exists(previous_record) && current_record->key > record->key)) {
            add_to_overflow(data, overflow, data->page->record_index - 1, record);
            break;
        }
        // Parent is the current (last in the page) record
        else if (i == RECORD_COUNT_PER_PAGE - 1) {
            add_to_overflow(data, overflow, data->page->record_index, record);
            break;
        }
        record_index = data->page->record_index;
        copy_record(current_record, previous_record);
        current_record = get_next_record(data);
    }
    if (RECORD_COUNT_PER_PAGE * overflow->number_of_pages == overflow->number_of_records)
        reorganise(indexes, data, overflow, ALPHA);
    destroy_index(index);
    destroy_record(previous_record);
    return 0;
}

void add_to_overflow(data_t *data, data_t *overflow, int parent_record_index, record_t *child) {
    int index_in_file = overflow->free_index;
    (overflow->free_index)++;
    // Add record to the index_in_file
    int append_overflow = 1;
    // Update the pointer
    if (data->page->records[parent_record_index]->overflow_pointer == EMPTY_VALUE) {
        // Add pointer to data record
        data->page->records[parent_record_index]->overflow_pointer = index_in_file;
        write_data_page(data);
    } else {
        // Add pointer to overflow record
        append_overflow = update_chain(data, overflow, parent_record_index, index_in_file, child);
    }
    if (append_overflow) {
        overflow->page_index = get_page_index(index_in_file);
        overflow->page->record_index = get_record_index(index_in_file);
        read_data_page(overflow);
        int page_was_written = add_record(overflow, child);
        if (!page_was_written)
            write_data_page(overflow);
        overflow->last_written_page_index = overflow->page_index;
        (overflow->number_of_records)++;
    }
}

int update_chain(data_t *data, data_t *overflow, int parent_record_index, int record_pointer, record_t *record) {
    // TODO: Refactor
    int current_pointer = data->page->records[parent_record_index]->overflow_pointer;
    overflow->page_index = get_page_index(current_pointer);
    overflow->page->record_index = get_record_index(current_pointer);
    read_data_page(overflow);
    // TODO: These could be uninitialized (SEGFAULT incoming)
    int previous_page_index, previous_record_index;
    int found_space = 0;
    record_t *current_record = overflow->page->records[overflow->page->record_index];
    // Look for empty and valid pointer space
    while (current_record->overflow_pointer != EMPTY_VALUE) {
        if (current_record->key < record->key) {
            // We found a potential space to insert into
            previous_page_index = overflow->page_index;
            previous_record_index = overflow->page->record_index;
            found_space = 1;
        }
        if (current_record->key > record->key && !found_space) {
            // We need to insert the record before a record, which is first in a chain
            record->overflow_pointer = current_pointer;
            write_data_page(overflow);
            overflow->last_written_page_index = overflow->page_index;
            // Update data record pointer
            data->page->records[parent_record_index]->overflow_pointer = record_pointer;
            write_data_page(data);
            return 1;
        }
        if (current_record->key == record->key) {
            printf("WARNING: Record with key %d already exists!\n", record->key);
            return 0;
        }
        get_next_in_chain(overflow, current_record->overflow_pointer);
        current_record = overflow->page->records[overflow->page->record_index];
        if (current_record->key > record->key && found_space) {
            // We need to insert a record between two different records
            overflow->page_index = previous_page_index;
            overflow->page->record_index = previous_record_index;
            read_data_page(overflow);
            record->overflow_pointer = overflow->page->records[overflow->page->record_index]->overflow_pointer;
            overflow->page->records[overflow->page->record_index]->overflow_pointer = record_pointer;
            write_data_page(overflow);
            overflow->last_written_page_index = overflow->page_index;
            return 1;
        } else {
            found_space = 0;
        }
    }
    if (current_record->key > record->key && !found_space) {
        // We need to insert the record before a record, which is first in a chain
        record->overflow_pointer = (overflow->page->record_index + (overflow->page_index * RECORD_COUNT_PER_PAGE));
        write_data_page(overflow);
        // Update data record pointer
        data->page->records[parent_record_index]->overflow_pointer = record_pointer;
        write_data_page(data);
        return 1;
    }
    if (current_record->key == record->key) {
        printf("WARNING: Record with key %d already exists!\n", record->key);
        return 0;
    }
    // Insert the pointer
    current_record->overflow_pointer = record_pointer;
    write_data_page(overflow);
    return 1;
}

void get_next_in_chain(data_t *overflow, int pointer) {
    // TODO: Load the page where the record is (check if the load is currently loaded to not load twice the same page)
    if (overflow->page_index != get_page_index(pointer)) {
        overflow->page_index = get_page_index(pointer);
        read_data_page(overflow);
    }
    overflow->page->record_index = get_record_index(pointer);
}

int find_free_space(data_t *overflow) {
    overflow->page_index = 0;
    overflow->page->record_index = 0;
    read_data_page(overflow);
    int index_in_file = 0;
    record_t *record = get_current_record(overflow);
    while (!is_data_at_end(overflow) && record_exists(record)) {
        record = get_next_record(overflow);
        index_in_file++;
    }
    if (!record_exists(record) && !is_data_at_end(overflow)) {
        // Empty space
        return index_in_file;
    } else {
        // Overflow is full
        return ERROR_VALUE;
    }
}

int get_page_index(int record_pointer) {
    return record_pointer / RECORD_COUNT_PER_PAGE;
}

int get_record_index(int record_pointer) {
    return record_pointer % RECORD_COUNT_PER_PAGE;
}

record_t *find_record(indexes_t *indexes, data_t *data, data_t *overflow, int record_key) {
    // TODO: Refactor (task for students)
    // TODO: Not working
    index_t *index = find_data_page_index(indexes, record_key);
    data->page_index = index->data_page_index;
    destroy_index(index);
    data->page->record_index = 0;
    read_data_page(data);
    read_data_page(overflow);
    record_t *previous_record = create_record(EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
    record_t *current_record = get_current_record(data);
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        if (current_record->key == record_key) {
            return current_record;
        }
        // Traverse through
        else if (record_exists(previous_record) && previous_record->key < record_key && record_key < current_record->key) {
            if (previous_record->overflow_pointer == EMPTY_VALUE) {
                destroy_record(previous_record);
                return NULL;
            }
            get_next_in_chain(overflow, previous_record->overflow_pointer);
            current_record = overflow->page->records[overflow->page->record_index];
            if (current_record->key == record_key) {
                printf("INFO: Record found in overflow.\n");
                destroy_record(previous_record);
                return current_record;
            }
            while (current_record->overflow_pointer != EMPTY_VALUE) {
                if (current_record->key == record_key) {
                    printf("INFO: Record found in overflow.\n");
                    return current_record;
                } else if (current_record->key > record_key) {
                    destroy_record(previous_record);
                    return NULL;
                }
                get_next_in_chain(overflow, current_record->overflow_pointer);
                current_record = overflow->page->records[overflow->page->record_index];
            }
        }
        if (current_record->key == record_key) {
            return current_record;
        } else if (current_record->key > record_key) {
            destroy_record(previous_record);
            return NULL;
        } else if (current_record->key > record_key && i == RECORD_COUNT_PER_PAGE - 1) {
            destroy_record(previous_record);
            return NULL;
        }
        copy_record(current_record, previous_record);
        current_record = get_next_record(data);
    }
    destroy_record(previous_record);
    return NULL;
}

record_t *get_by_pointer(data_t *data, int pointer) {
    int temp_page_index = data->page_index;
    data->page_index = get_page_index(pointer);
    data->page->record_index = get_record_index(pointer);
    read_data_page(data);
    record_t *record = get_current_record(data);
    data->page_index = temp_page_index;
    return record;
}

void reorganise(indexes_t *indexes, data_t *data, data_t *overflow, double alpha) {
    int max_record_count_per_page = floor(RECORD_COUNT_PER_PAGE * alpha);
    // TODO: max_record_count_per_page can be 0 - exception, check if double conversion is needed
    int number_of_data_pages = ceil((double)(data->number_of_records + overflow->number_of_records) / max_record_count_per_page);
    int number_of_overflow_pages = ceil(number_of_data_pages * OVERFLOW_TO_DATA_PAGES_RATIO);
    if (number_of_data_pages == 0)
        return;
    data_t *temp_data = create_data(TEMP_DATA_FILENAME, number_of_data_pages);
    data_t *temp_overflow = create_data(TEMP_OVERFLOW_FILENAME, number_of_overflow_pages);
    indexes_t *temp_indexes = create_indexes(TEMP_INDEXES_FILENAME);
    record_t *current_record = create_record(EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE, EMPTY_VALUE);
    int processed_records = 0;
    int is_in_overflow = 0;
    int current_page = 0;
    int records_on_current_page = 0;
    int current_primary_record_offset = 0;
    while (processed_records != data->number_of_records + overflow->number_of_records) {
        if (!is_in_overflow) {
            copy_record(get_by_pointer(data, current_primary_record_offset), current_record);
        }
        if (!record_exists(current_record)) {
            current_primary_record_offset++;
            continue;
        }
        if (records_on_current_page >= max_record_count_per_page) {
            current_page++;
            records_on_current_page = 0;
        }
        if (records_on_current_page == 0) {
            index_t *index = create_index(current_page, current_record->key);
            add_index(temp_indexes, index);
            write_indexes_page(temp_indexes);
            destroy_index(index);
        }
        temp_data->page_index = current_page;
        // read_data_page(temp_data);
        int overflow_pointer = current_record->overflow_pointer;
        current_record->overflow_pointer = EMPTY_VALUE;
        insert_record(temp_indexes, temp_data, temp_overflow, current_record);
        records_on_current_page++;
        processed_records++;
        if (overflow_pointer == EMPTY_VALUE) {
            current_primary_record_offset++;
            is_in_overflow = 0;
        } else {
            is_in_overflow = 1;
            overflow->page_index = get_page_index(overflow_pointer);
            overflow->page->record_index = get_record_index(overflow_pointer);
            read_data_page(overflow);
            copy_record(get_current_record(overflow), current_record);
        }
    }
    destroy_record(current_record);
    temp_data->number_of_pages = current_page + 1;
    data->number_of_pages = temp_data->number_of_pages;
    data->number_of_records = processed_records;
    overflow->number_of_pages = temp_overflow->number_of_pages;
    overflow->number_of_records = 0;
    overflow->free_index = 0;
    delete_file(DATA_FILENAME);
    delete_file(OVERFLOW_FILENAME);
    delete_file(INDEXES_FILENAME);
    rename_file(TEMP_DATA_FILENAME, DATA_FILENAME);
    rename_file(TEMP_OVERFLOW_FILENAME, OVERFLOW_FILENAME);
    rename_file(TEMP_INDEXES_FILENAME, INDEXES_FILENAME);
    destroy_data(temp_data);
    destroy_data(temp_overflow);
    destroy_indexes(temp_indexes);
}