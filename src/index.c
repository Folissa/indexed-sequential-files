#include "index.h"

index_t *create_index(int data_page_index, int key) {
    index_t *index = (index_t *)malloc(sizeof(index_t));
    initialize_index(index, data_page_index, key);
    return index;
}

void initialize_index(index_t *index, int data_page_index, int key) {
    index->data_page_index = data_page_index;
    index->key = key;
}

void destroy_index(index_t *index) {
    free(index);
}

int index_exists(index_t *index) {
    if (index->data_page_index == EMPTY_VALUE)
        return 0;
    return 1;
}

void copy_index(index_t *source, index_t *destination) {
    destination->data_page_index = source->data_page_index;
    destination->key = source->key;
}

void print_index(index_t *index) {
    char format[INPUT_BUFFER_SIZE] = "";
    for (int i = 0; i < INT_WIDTH; i++) {
        strcat(format, "#");
    }
    if (index_exists(index)) {
        printf("DATA_PAGE_INDEX: %*d, KEY: %*d\n", INT_WIDTH, index->data_page_index, INT_WIDTH, index->key);
    } else
        printf("DATA_PAGE_INDEX: %s, KEY: %s\n", format, format);
}