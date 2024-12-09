#include "indexes.h"

indexes_t *create_indexes(char *filename) {
    init_file(filename);
    indexes_t *indexes = (indexes_t *)malloc(sizeof(indexes_t));
    initialize_indexes(indexes, filename);
    indexes->page = create_indexes_page();
    return indexes;
}

void initialize_indexes(indexes_t *indexes, char *filename) {
    indexes->filename = filename;
    indexes->page_index = 0;
    indexes->writes = 0;
    indexes->reads = 0;
}

void destroy_indexes(indexes_t *indexes) {
    destroy_indexes_page(indexes->page);
    free(indexes);
}

void handle_full_indexes_page(indexes_t *indexes, int write, int read) {
    if (is_indexes_page_full(*(indexes->page))) {
        if (write)
            write_indexes_page(indexes);
        indexes->page_index++;
        indexes->page->index_index = 0;
        if (read)
            read_indexes_page(indexes);
    }
}

void write_index(FILE *file, index_t *index, int index_index) {
    int index_size = INDEX_PARAMETERS_COUNT * INT_WIDTH;
    long int index_offset = index_index * index_size;
    if (fseek(file, index_offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        return;
    }
    fprintf(file, "%0*d%0*d",
        INT_WIDTH, index->data_page_index,
        INT_WIDTH, index->key);
}

void write_indexes_page(indexes_t *indexes) {
    FILE *file = open_file(indexes->filename, "r+");
    for (int i = 0; i < INDEXES_COUNT_PER_PAGE; i++) {
        int index_index = indexes->page_index * INDEXES_COUNT_PER_PAGE + i;
        write_index(file, indexes->page->indexes[i], index_index);
        initialize_index(indexes->page->indexes[i], EMPTY_VALUE, EMPTY_VALUE);
    }
    close_file(file);
    (indexes->writes)++;
}

void read_index(indexes_t *indexes, char *buffer, int index_index) {
    int index_size = INDEX_PARAMETERS_COUNT * INT_WIDTH;
    int index_offset = index_index * index_size;
    if (buffer[index_offset] == '\0') {
        // Situation: there are indexes to read, but it will not fill the whole page,
        // so we initalize as they do not exist
        initialize_index(indexes->page->indexes[index_index], EMPTY_VALUE, EMPTY_VALUE);
        return;
    }
    char temp[INT_WIDTH + NULL_CHARACTER_SIZE];
    temp[INT_WIDTH] = '\0';
    memcpy(temp, buffer + index_offset + FIRST_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    indexes->page->indexes[index_index]->data_page_index = atoi(temp);
    memcpy(temp, buffer + index_offset + SECOND_PARAMETER_OFFSET * INT_WIDTH, INT_WIDTH);
    indexes->page->indexes[index_index]->key = atoi(temp);
}

void read_indexes_page(indexes_t *indexes) {
    FILE *file = open_file(indexes->filename, "r");
    
    int page_index = indexes->page_index;
    int index_size = INDEX_PARAMETERS_COUNT * INT_WIDTH;
    int indexes_size = INDEXES_COUNT_PER_PAGE * index_size;
    long int page_offset = page_index * indexes_size;
    if (fseek(file, page_offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
    }
    char buffer[indexes_size + NULL_CHARACTER_SIZE];
    for (int i = 0; i < indexes_size + NULL_CHARACTER_SIZE; i++) {
        buffer[i] = '\0';
    }
    if (fread(buffer, sizeof(char), indexes_size, file)) {
        for (int i = 0; i < INDEXES_COUNT_PER_PAGE; i++)
            read_index(indexes, buffer, i);
        (indexes->reads)++;

    } else {
        // Reached EOF, mark whole page as non existing
        for (int i = 0; i < INDEXES_COUNT_PER_PAGE; i++) {
            initialize_index(indexes->page->indexes[i], EMPTY_VALUE, EMPTY_VALUE);
        }
    }
    close_file(file);
}

void add_index(indexes_t *indexes, index_t *index) {
    if (index_exists(indexes->page->indexes[indexes->page->index_index])) {
        (indexes->page->index_index)++;
    }
    handle_full_indexes_page(indexes, 1, 0);
    copy_index(index, indexes->page->indexes[indexes->page->index_index]);
}

index_t *get_next_index(indexes_t *indexes) {
    indexes->page->index_index++;
    handle_full_indexes_page(indexes, 0, 1);
    return indexes->page->indexes[indexes->page->index_index];
}

void insert_dummy_indexes(indexes_t *indexes) {
    index_t *index = create_index(0, 450);
    add_index(indexes, index);
    destroy_index(index);

    index = create_index(1, 1000);
    add_index(indexes, index);
    destroy_index(index);

    index = create_index(2, 2000);
    add_index(indexes, index);
    destroy_index(index);

    write_indexes_page(indexes);
}


void move_indexes_to_start(indexes_t *indexes) {
    indexes->page->index_index = 0;
    indexes->page_index = 0;
    read_indexes_page(indexes);
}

index_t *get_current_index(indexes_t *indexes) {
    return indexes->page->indexes[indexes->page->index_index];
}

index_t *find_data_page_index(indexes_t *indexes, int record_key) {
    int data_page_index;
    move_indexes_to_start(indexes);
    index_t *previous_index = create_index(EMPTY_VALUE, EMPTY_VALUE);
    index_t *current_index = get_current_index(indexes);
    index_t *result_index = create_index(EMPTY_VALUE, EMPTY_VALUE);
    while (!is_indexes_at_end(indexes)) {
        if (index_exists(previous_index) && current_index->key > record_key) {
            copy_index(previous_index, result_index);
            // data_page_index = previous_index->data_page_index;
            destroy_index(previous_index);
            (indexes->page->index_index)--;
            return result_index;
            // return data_page_index;
        }
        copy_index(current_index, previous_index);
        current_index = get_next_index(indexes);
    }
    if (index_exists(previous_index))
        copy_index(previous_index, result_index);
        // data_page_index = previous_index->data_page_index;
    else
        copy_index(current_index, result_index);
        // data_page_index = current_index->data_page_index;
    destroy_index(previous_index);
    return result_index;
    // return data_page_index;
}

int is_indexes_at_end(indexes_t *indexes) {
    return !(index_exists(indexes->page->indexes[indexes->page->index_index]));
}
