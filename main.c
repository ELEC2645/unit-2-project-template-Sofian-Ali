#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_calc.h"
#include "input_guard.h"
#include "signals.h"
#include "comms.h"

/* ---------- Menu / Calculator Functions ---------- */
void main_menu(void);
int user_input(void);
void print_menu(void);
void select_menu(int value);

int main(void) {
    while (1) main_menu(); // continuous menu
    return 0;
}

void main_menu(void){
    print_menu();
    int value = user_input();
    select_menu(value);
}

int user_input(void){
    int input;
    double temp;
    printf("\nEnter a value 1-4:\n");

    while (1) {
        printf("> ");
        if (get_valid_number(&temp,0) == 0 && temp >= 1 && temp <= 4 && temp == (int)temp) {
            input = (int)temp;
            return input;
        }

        printf("Invalid choice. Enter a number between 1 and 4.\n");
    }
}

void print_menu(void){
    printf("\n---------------- Main Menu ----------------------\n");
    printf("|\t1. E-Calc: Electrical Calculator \t|\n");
    printf("|\t2. Signal Analysis & Plotting    \t|\n");
    printf("|\t3. Communication Tools           \t|\n");
    printf("|\t4. Exit                          \t|\n");
    printf("-------------------------------------------------\n");
}

void select_menu(int value){
    switch (value) {
        case 1: runECalcMenu(); break;
        case 2: SignalsMenu(); break;
        case 3: CommsMenu(); break;
        case 4:
            printf("Exiting program...\n");
            exit(0);
        default:
            printf("Invalid selection.\n");
    }
}