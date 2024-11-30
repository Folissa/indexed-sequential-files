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
