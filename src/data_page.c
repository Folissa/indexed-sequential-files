#include "data_page.h"

data_page_t *create_data_page() {
    data_page_t *page = (data_page_t *)malloc(sizeof(data_page_t));
    initialize_data_page(page);
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        page->records[i] = create_record(DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
    }
    return page;
}

void initialize_data_page(data_page_t *page) {
    page->record_index = 0;
}

void destroy_data_page(data_page_t *page) {
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        destroy_record(page->records[i]);
    }
    free(page);
}

int is_data_page_full(data_page_t page) {
    return page.record_index >= RECORD_COUNT_PER_PAGE;
}