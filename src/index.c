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
    if (index->data_page_index == DEFAULT_VALUE)
        return 0;
    return 1;
}

void copy_index(index_t *source, index_t *destination) {
    destination->data_page_index = source->data_page_index;
    destination->key = source->key;
}