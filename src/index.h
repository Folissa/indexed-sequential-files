#pragma once

#ifndef INDEX_H
#define INDEX_H

#include <stdlib.h>

typedef struct {
    int data_page_index;
    int key;
} index_t;

index_t *create_index();

void initialize_index(index_t *index);

void destroy_index(index_t *index);


#endif // INDEX_H