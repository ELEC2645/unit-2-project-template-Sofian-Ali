#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_guard.h"

static void remove_previous_input(void) { // clears leftover input 
    int ch;
    while ((ch = getchar()) != '\n') { }
}

// SAFE NUMBER INPUT
static int ask_number(double *out_number) {
    char buffer[128];
    char *endptr;

    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Input error. Try again:\n");
            continue;
        }

        /* Flush remainder if too long */
        if (buffer[strlen(buffer) - 1] != '\n') {
            int ch;
            while ((ch = getchar()) != '\n');
        }

        /* Convert string to double */
        *out_number = strtod(buffer, &endptr);

        /* No valid digits were found */
        if (endptr == buffer) {
            printf("Invalid number. Try again:\n");
            continue;
        }

        return 0;  // success
    }
}

// Waiting for enter to return to the menu
void wait_for_enter(void) {
    int ch;

    printf("\nPress Enter to continue..\n");

    ch = getchar();
    if (ch != '\n') {
        while ((ch = getchar()) != '\n');
    }
}

// Checking if input is zero
static int validate_not_zero(double value) {
    if (value == 0.0) {
        printf("Error: Division by zero is not allowed.\n");
        return 0;  // invalid
    }
    return 1;      // valid
}

// Range Check
static int validate_range(double value) {
    double limit = 1e12;  

    if (value > limit || value < -limit) {
        printf("Error: Number too large. Try a smaller value.\n");
        return 0;  // invalid
    }

    return 1;       // valid
}

int get_valid_number(double *out_value, int allow_zero) {
    double value;
    int status;

    while (1) {
        status = ask_number(&value);

        if (status != 0) {
            printf("Invalid number. Please try again:\n");
            continue;
        }

        if (!validate_range(value)) {
            // error message already printed in validate_range
            continue;
        }

        if (!allow_zero && value == 0.0) {
            printf("Error: Zero is not allowed here.\n");
            continue;
        }
        *out_value = value;
        return 0; // success with fully validated value
    }
}