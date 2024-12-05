#pragma once

#define RECORD_PARAMETERS_COUNT (sizeof(record_t) / sizeof(int))
#define RECORD_COUNT_PER_PAGE 4
#define INDEX_PARAMETERS_COUNT (sizeof(index_t) / sizeof(int))
#define INDEXES_COUNT_PER_PAGE RECORD_COUNT_PER_PAGE

#define NULL_CHARACTER_SIZE 1
#define INPUT_BUFFER_SIZE 64
#define INT_WIDTH 5
#define EMPTY_VALUE -1
#define NUMBER_OF_DATA_PAGES 3
#define NUMBER_OF_OVERFLOW_PAGES 2
#define ERROR_VALUE -1

#define FIRST_PARAMETER_OFFSET 0
#define SECOND_PARAMETER_OFFSET 1
#define THIRD_PARAMETER_OFFSET 2
#define FOURTH_PARAMETER_OFFSET 3
#define FIFTH_PARAMETER_OFFSET 4

#define INPUT_FILENAME "input.txt"
#define INDEXES_FILENAME "indexes.txt"
#define DATA_FILENAME "data.txt"
#define OVERFLOW_FILENAME "overflow.txt"
