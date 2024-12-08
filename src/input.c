#include "input.h"

void print_prompt() {
    printf("> ");
}

void clear_input_buffer() {
    while (getchar() != '\n')
        ;
}

void clear_input_array(char *input, size_t size) {
    memset(input, 0, size);
}

void print_help() {
    printf("Available commands:\n");
    printf("HELP                                                      - print help\n");
    printf("INSERT key mass specific_heat_capacity temperature_change - insert a new record with a given key\n");
    printf("GET key                                                   - get a record with a given key\n");
    printf("EXIT                                                      - exit the program\n");
}

void print_invalid_input_message(int invalid_format) {
    if (invalid_format) {
        printf("Invalid input format. Type HELP to see available commands.\n");
    } else {
        printf("Invalid input. Type HELP to see available commands.\n");
    }
}

void load_operations_from_file(indexes_t *indexes, data_t *data, data_t *overflow) {
    FILE *file= open_file(INPUT_FILENAME, "r");
    char input[INPUT_BUFFER_SIZE];
    while (fgets(input, sizeof(input), file)) {
        input[strcspn(input, "\n")] = '\0';
        char *command = strtok(input, " ");
        if (command == NULL) {
            print_invalid_input_message(0);
            continue;
        }
        if (strcmp(command, "INSERT") == 0) {
            int key, mass, specific_heat_capacity, temperature_change;
            char *args = strtok(NULL, "");
            if (args != NULL && sscanf(args, "%d %d %d %d", &key, &mass, &specific_heat_capacity, &temperature_change) == 4) {
                record_t *record = create_record(key, mass, specific_heat_capacity, temperature_change, EMPTY_VALUE);
                insert_record(indexes, data, overflow, record);
                #ifndef DDEBUG
                #define DDEBUG
                print_data(data);
                print_data(overflow);
                #endif // DDEBUG
            } else {
                print_invalid_input_message(1);
            }
        } else {
            print_invalid_input_message(0);
        }
    }
    close_file(file);
    print_data(data);
    print_data(overflow);
}
void input_operations_from_keyboard(indexes_t *indexes, data_t *data, data_t *overflow) {
    clear_input_buffer();
    char input[INPUT_BUFFER_SIZE];
    int exit = 0;
    while (!exit) {
        clear_input_array(input, sizeof(input));
        print_prompt();
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Error reading input.\n");
            clear_input_buffer();
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        char *command = strtok(input, " ");
        if (command == NULL) {
            print_invalid_input_message(0);
            continue;
        }
        if (strcmp(command, "HELP") == 0) {
            print_help();
        } else if (strcmp(command, "INSERT") == 0) {
            int key, mass, specific_heat_capacity, temperature_change;
            char *args = strtok(NULL, "");
            if (args != NULL && sscanf(args, "%d %d %d %d", &key, &mass, &specific_heat_capacity, &temperature_change) == 4) {
                record_t *record = create_record(key, mass, specific_heat_capacity, temperature_change, EMPTY_VALUE);
                insert_record(indexes, data, overflow, record);
            } else {
                print_invalid_input_message(1);
            }
        } else if (strcmp(command, "GET") == 0) {
            int key;
            char *args = strtok(NULL, "");
            if (args != NULL && sscanf(args, "%d", &key) == 1) {
                record_t *record = find_record(indexes, data, overflow, key);
                if (record != NULL)
                    print_record(record);
            } else {
                print_invalid_input_message(1);
            }
        } else if (strcmp(command, "EXIT") == 0) {
            exit = 1;
            continue;
        } else {
            print_invalid_input_message(0);
            continue;
        }
        print_data(data);
        print_data(overflow);
    }
}

void print_menu() {
    printf("1. Load test operations from file\n");
    printf("2. Input operations from keyboard\n");
    printf("3. Exit\n");
}
