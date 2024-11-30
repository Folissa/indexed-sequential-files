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

void input_records() {
}

void load_operations_from_file() {
}

void print_help() {
    printf("Available commands:\n");
    printf("HELP                                                      - print help\n");
    printf("INSERT key mass specific_heat_capacity temperature_change - insert a new record with a given key\n");
    printf("GET key                                                   - get a record with a given key\n");
    printf("EXIT                                                      - exit the program\n");
}

void print_inalid_input_message(int invalid_format) {
    if (invalid_format) {
        printf("Invalid input format. Type HELP to see available commands.\n");
    } else {
        printf("Invalid input. Type HELP to see available commands.\n");
    }
}

#ifdef DEBUG
void print_ok() {
    printf("OK\n");
}
#endif // DEBUG

void input_operations_from_keyboard() {
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
            print_inalid_input_message(0);
            continue;
        }
        if (strcmp(command, "HELP") == 0) {
            print_help();
            continue;
        } else if (strcmp(command, "INSERT") == 0) {
            int key, value1, value2, value3;
            char *args = strtok(NULL, ""); // Get the rest of the input after "INSERT"
            if (args != NULL && sscanf(args, "%d %d %d %d", &key, &value1, &value2, &value3) == 4) {
                print_ok();
            } else {
                print_inalid_input_message(1);
            }
            continue;
        } else if (strcmp(command, "GET") == 0) {
            int key;
            char *args = strtok(NULL, "");
            if (args != NULL && sscanf(args, "%d", &key) == 1) {
                print_ok();
            } else {
                print_inalid_input_message(1);
            }
            continue;
        } else if (strcmp(command, "EXIT") == 0) {
            exit = 1;
        } else {
            print_inalid_input_message(0);
        }
    }
}

void print_menu() {
    printf("1. Load test operations from file\n");
    printf("2. Input operations from keyboard\n");
    printf("3. Exit\n");
}

void choose_program_mode() {
    int exit = 0;
    int choice;
    while (!exit) {
        print_menu();
        print_prompt();
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            load_operations_from_file();
            exit = 1;
            break;
        case 2:
            input_operations_from_keyboard();
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
}
