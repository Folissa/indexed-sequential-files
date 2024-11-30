#pragma once

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "tape.h"

// TODO: Record insert
// TODO: Record read
// TODO: File browsing (on demand (?), after each operation), so we can check where the record is (in the main area, or in the overflow area)
// TODO: After each operation, the number of reads from, and writes to a file are printed out
// DONE: Input implemented from the keyboard as an interactive program
// TODO: Input implemmented from the file - it should be any sequence of opearations like adding, updating, and removing records
// TODO: Reorganistion of a file (on demand, and automatically)

int main() {
    int records_count = 0;

    tape_t *indexes = create_tape(INDEXES_FILENAME);
    tape_t *data = create_tape(DATA_FILENAME);

    int exit = 0;
    int choice;
    while (!exit) {
        print_menu();
        print_prompt();
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            load_operations_from_file(indexes, data);
            exit = 1;
            break;
        case 2:
            input_operations_from_keyboard(indexes, data);
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

    destroy_tape(indexes);
    destroy_tape(data);
    return 0;
}
