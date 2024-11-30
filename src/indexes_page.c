#include "indexes_page.h"

indexes_page_t *create_indexes_page() {
    indexes_page_t *page = (indexes_page_t *)malloc(sizeof(indexes_page_t));
    initialize_indexes_page(page);
    for (int i = 0; i < INDEXES_COUNT_PER_PAGE; i++) {
        page->indexes[i] = create_index();
    }
    return page;
}

void initialize_indexes_page(indexes_page_t *page) {
    page->index_index = 0;
}

void destroy_indexes_page(indexes_page_t *page) {
    for (int i = 0; i < INDEXES_COUNT_PER_PAGE; i++) {
        destroy_index(page->indexes[i]);
    }
    free(page);
}

int is_indexes_page_full(indexes_page_t page) {
    return page.index_index >= INDEXES_COUNT_PER_PAGE;
}