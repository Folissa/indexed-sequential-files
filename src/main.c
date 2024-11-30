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
// TODO: Input implemented from the keyboard as an interactive program
// TODO: Input implemmented from the file - it should be any sequence of opearations like adding, updating, and removing records
// TODO: Reorganistion of a file (on demand, and automatically)

int main() {
    int records_count = 0;

    tape_t *indexes = create_tape(INDEXES_FILENAME);
    tape_t *data = create_tape(DATA_FILENAME);

    choose_program_mode();

    destroy_tape(indexes);
    destroy_tape(data);
    return 0;
}
