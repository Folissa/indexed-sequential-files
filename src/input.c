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
    printf("REORGANISE                                                - reorganise current data and overflow\n");
    printf("SHOW                                                      - show the values of data and overflow\n");
    printf("EXIT                                                      - exit the program\n");
}

void print_invalid_input_message(int invalid_format) {
    if (invalid_format) {
        printf("Invalid input format. Type HELP to see available commands.\n");
    } else {
        printf("Invalid input. Type HELP to see available commands.\n");
    }
}

void print_data_and_overflow(data_t *data, data_t *overflow) {
    printf("==============================================================================================================\n");
    print_data(data);
    print_data(overflow);
    printf("==============================================================================================================\n");
}

void load_operations_from_file(indexes_t *indexes, data_t *data, data_t *overflow) {
    FILE *file = open_file(INPUT_FILENAME, "r");
    char input[INPUT_BUFFER_SIZE];
    double mean_saves = 0;
    double mean_loads = 0;
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
                data->reads = 0;
                data->writes = 0;
                overflow->reads = 0;
                overflow->writes = 0;
                insert_record(indexes, data, overflow, record);
                if (mean_saves == 0) {
                    mean_saves = data->writes + overflow->writes;
                    mean_loads = data->reads + overflow->reads;
                } else {
                    mean_saves = (mean_saves + data->writes + overflow->writes) / 2;
                    mean_loads = (mean_loads + data->reads + overflow->reads) / 2;
                }
                destroy_record(record);
                print_indexes(indexes);
                print_data_and_overflow(data, overflow);
                printf("Writes: %d\n", data->writes + overflow->writes);
                printf("Reads: %d\n", data->reads + overflow->reads);
            } else {
                print_invalid_input_message(1);
            }
        } else {
            print_invalid_input_message(0);
        }
    }
    close_file(file);
    print_indexes(indexes);
    print_data_and_overflow(data, overflow);
    printf("Mean saves: %f\n", mean_saves);
    printf("Mean loads: %f\n", mean_loads);
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
            continue;
        } else if (strcmp(command, "INSERT") == 0) {
            int key, mass, specific_heat_capacity, temperature_change;
            char *args = strtok(NULL, "");
            if (args != NULL && sscanf(args, "%d %d %d %d", &key, &mass, &specific_heat_capacity, &temperature_change) == 4) {
                record_t *record = create_record(key, mass, specific_heat_capacity, temperature_change, EMPTY_VALUE);
                insert_record(indexes, data, overflow, record);
                destroy_record(record);
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
        } else if (strcmp(command, "REORGANISE") == 0) {
            reorganise(indexes, data, overflow, ALPHA);
        } else if (strcmp(command, "SHOW") == 0) {
            print_data_and_overflow(data, overflow);
            continue;
        } else if (strcmp(command, "EXIT") == 0) {
            exit = 1;
            continue;
        } else {
            print_invalid_input_message(0);
            continue;
        }
        print_indexes(indexes);
        print_data_and_overflow(data, overflow);
        printf("Writes: %d\n", data->writes + overflow->writes);
        printf("Reads: %d\n", data->reads + overflow->reads);
    }
}

void print_menu() {
    printf("1. Load test operations from file\n");
    printf("2. Input operations from keyboard\n");
    printf("3. Exit\n");
}
