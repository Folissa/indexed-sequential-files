#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "indexes.h"
#include "data.h"

// DONE: Record insert
// DONE: Record read
// DONE: File browsing (on demand (?), after each operation), so we can check where the record is (in the main area, or in the overflow area)
// TODO: After each operation, the number of reads from, and writes to a file are printed out
// DONE: Input implemented from the keyboard as an interactive program
// DONE: Input implemmented from the file - it should be any sequence of opearations like adding, updating, and removing records
// TODO: Reorganistion of a file (on demand, and automatically)

int main() {
    int records_count = 0;
    indexes_t *indexes = create_indexes(INDEXES_FILENAME);
    data_t *data = create_data(DATA_FILENAME, NUMBER_OF_DATA_PAGES);
    data_t *overflow = create_data(OVERFLOW_FILENAME, NUMBER_OF_OVERFLOW_PAGES);
    insert_dummy_indexes(indexes);
    // insert_dummy_data(indexes, data, overflow);
    print_data(data);
    print_data(overflow);
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
            clear_input_buffer();
            break;
        }
    };
    destroy_indexes(indexes);
    destroy_data(data);
    return 0;
}
