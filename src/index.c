#include "index.h"

index_t *create_index() {
    index_t *index = (index_t *)malloc(sizeof(index_t));
    initialize_index(index);
    return index;
}

void initialize_index(index_t *index) {

}

void destroy_index(index_t *index) {
    free(index);
}
