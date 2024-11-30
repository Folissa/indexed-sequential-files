#include "page.h"

page_t *create_page() {
    page_t *page = (page_t *)malloc(sizeof(page_t));
    initialize_page(page);
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        // TODO: Fix key
        page->records[i] = create_record(0);
    }
    return page;
}

void initialize_page(page_t *page) {
    page->record_index = 0;
}

void destroy_page(page_t *page) {
    for (int i = 0; i < RECORD_COUNT_PER_PAGE; i++) {
        destroy_record(page->records[i]);
    }
    free(page);
}

int is_page_full(page_t page) {
    return page.record_index >= RECORD_COUNT_PER_PAGE;
}