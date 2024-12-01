#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "indexes.h"
#include "data.h"

// TODO: Record insert
// TODO: Record read
// DONE: File browsing (on demand (?), after each operation), so we can check where the record is (in the main area, or in the overflow area)
// TODO: After each operation, the number of reads from, and writes to a file are printed out
// DONE: Input implemented from the keyboard as an interactive program
// TODO: Input implemmented from the file - it should be any sequence of opearations like adding, updating, and removing records
// TODO: Reorganistion of a file (on demand, and automatically)

int main() {
    int records_count = 0;
    indexes_t *indexes = create_indexes(INDEXES_FILENAME);
    data_t *data = create_data(DATA_FILENAME);
    data_t *overflow = create_data(OVERFLOW_FILENAME);
    insert_dummy_indexes(indexes);
    insert_dummy_data(indexes, data);
    print_data(data);
    print_data(overflow);
#ifndef DEBUG
#define DEBUG
    int index;
    record_t *record; 
    record = create_record(0, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
    record = create_record(1, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
    record = create_record(1000, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
    record = create_record(1005, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
    record = create_record(2000, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
    record = create_record(2005, 1, 1, 1);
    index = find_data_page_index(indexes, record);
    destroy_record(record);
#endif // DEBUG
    int exit = 0;
    int choice;
    while (!exit) {
        print_menu();
        print_prompt();
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            load_operations_from_file(indexes, data, overflow);
            exit = 1;
            break;
        case 2:
            input_operations_from_keyboard(indexes, data, overflow);
            exit = 1;
            break;
        case 3:
            exit = 1;
            break;
        default:
            printf("Invalid choice, please try again.\n");
            break;
        }
    };
    destroy_indexes(indexes);
    destroy_data(data);
    return 0;
}
