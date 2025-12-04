#include "stdio.h"
#include "e_calc.h" 
#include "input_guard.h"
#include <math.h> 

void db_menu(void);
void runECalcMenu(void);
void OhmsLawMenu(void);

// Solving for each variable
static void solve_voltage(void); // V=I*R
static void solve_current(void); // I=V/R
static void solve_resistance(void); // R=V/I
static void solve_power(void); // P=V*I

static void power_ratio_to_db(void); 
static void db_to_power_ratio(void); 
static void mw_to_dbm(void);
static void dbm_to_mw(void);

void runECalcMenu(void) {
  int choice; // Stores validated int input
  double userInput; // Stores input before validation

   while(1) {
        printf("\n--- E-Calc Menu ---\n");
        printf("What E-Calc do you want to use?\n");
        printf("1. Ohm's Law Tools (V, I, R, P)\n");
        printf("2. dB & Power Conversions\n");
        printf("3. Potential Divider calculation\n");
        printf("4. Back to Electrical Toolkit menu\n");
        printf("Choose 1-4:\n> ");
    
        if (get_valid_number(&userInput,0) == (int)userInput) { // Double input validation
          printf("The input is invalid. Please choose an integer between 1 to 5.\n");
          wait_for_enter();
        } 
        
    choice = (int)userInput; // Convert double to int after validating input is integer
 
    switch (choice) {
      case 1: OhmsLawMenu(); break;
      case 2: db_menu(); break;
      case 3: solve_resistance(); break;
      case 4: return;    
      default:
      printf("Invalid choice. Choose between 1 and 4.\n");
      wait_for_enter();  
    } 
}
}

void OhmsLawMenu(void){
  int choice; // Stores validated int input
  double userInput; // Stores input before validation

   while(1) {
        printf("\n--- Ohm's Law Calculator ---\n");
        printf("What do you want to calculate?\n");
        printf("1. Voltage   (V = I * R)\n");
        printf("2. Current   (I = V / R)\n");
        printf("3. Resistance(R = V / I)\n");
        printf("4. Power     (P = V * I)\n");
        printf("5. Back to Electrical Toolkit menu\n");
        printf("Choose 1-5:\n> ");
    
if (get_valid_number(&userInput,0) == (int)userInput) { // Double input validation
          printf("The input is invalid. Please choose an integer between 1 to 5.\n");
          wait_for_enter();
        } 
      
    choice = (int)userInput; // Convert double to int after validating input is integer
 
    switch (choice) {
      case 1: solve_voltage(); break;
      case 2: solve_current(); break;
      case 3: solve_resistance(); break;
      case 4: solve_power(); break;
      case 5: return;
      default:
      printf("Invalid choice. Choose between 1 and 5.\n");
      wait_for_enter();
    } 
}
}

void db_menu(void){
  int choice; // Stores validated int input
  double userInput; // Stores input before validation

   while(1) {
        printf("\n--- dB & Power Conversions ---\n");
        printf("1. Power ratio -> dB\n");
        printf("2. dB -> Power ratio\n");
        printf("3. mW -> dBm\n");
        printf("4. dBm -> mW\n");
        printf("5. Back to E-Calc menu\n");
        printf("Choose 1-5:\n> ");
    
if (get_valid_number(&userInput,0) == (int)userInput) { // Double input validation
          printf("The input is invalid. Please choose an integer between 1 to 5.\n");
          wait_for_enter();
        } 
      
    choice = (int)userInput; // Convert double to int after validating input is integer
 
    switch (choice) {
        case 1: power_ratio_to_db();   break;
        case 2: db_to_power_ratio();   break;
        case 3: mw_to_dbm();           break;
        case 4: dbm_to_mw();           break;
        case 5: return; // back to E-Calc menu
        } 
}
}


static void solve_voltage(void) {
  double I, R;

  printf("Enter the current in Amps\n");
  get_valid_number(&I,1);

  printf("Enter the resistance in Ohms\n");
  get_valid_number(&R,0);

  double V = I*R;
  printf("Voltage = %.2fV\n",V);
  wait_for_enter();
}

static void solve_current(void) {
  double V, R;

  printf("Enter the voltage in Volts\n");
  get_valid_number(&V,1);

  printf("Enter the resistance in Ohms\n");
  get_valid_number(&R,0);

  double I = V/R;
  printf("Current = %.2fA\n", I);
  wait_for_enter();  
}

static void solve_resistance(void) {
  double V, I;

  printf("Enter the voltage in Volts\n");
  get_valid_number(&V,1);

  printf("Enter the current in Amps\n");
  get_valid_number(&I,0);

  double R = V/I;
  printf("Resistance = %.2f Ohms\n", R);
  wait_for_enter();
}

static void solve_power(void) {
  double I,V;

  printf("Enter the voltage in Volts\n");
  get_valid_number(&V,1);

  printf("Enter the current in Amps\n");
  get_valid_number(&I,0);

  double P = I*V;
  printf("Power = %.2fW\n", P);
  wait_for_enter();
}

static void power_ratio_to_db(void) {
    double ratio, dB;

    printf("\n--- Power ratio -> dB ---\n");
    printf("Enter power ratio (Pout/Pin, > 0):\n> ");

    get_valid_number(&ratio, 0); // zero NOT allowed

    if (ratio <= 0) {
        printf("Error: Ratio must be > 0.\n");
        wait_for_enter();
        return;
    }

    dB = 10.0 * log10(ratio);

    printf("Result: %.2f dB\n", dB);
    wait_for_enter();
}

static void db_to_power_ratio(void) {
    double dB, ratio;

    printf("\n--- dB -> Power ratio ---\n");
    printf("Enter value in dB:\n> ");

    get_valid_number(&dB, 1); // any real number allowed

    ratio = pow(10, dB / 10);

    printf("Power ratio = %.2f\n", ratio);
    wait_for_enter();
}

static void mw_to_dbm(void) {
    double PmW, dBm;

    printf("\n--- mW -> dBm ---\n");
    printf("Enter Power in mW (> 0):\n> ");

    get_valid_number(&PmW, 0); // zero NOT allowed

    if (PmW <= 0.0) {
        printf("Error: power must be > 0 mW.\n");
        wait_for_enter();
        return;
    }

    dBm = 10 * log10(PmW);

    printf("Result: %.2f dBm\n", dBm);
    wait_for_enter();
}

static void dbm_to_mw(void) {
    double dBm, PmW;

    printf("\n--- dBm -> mW ---\n");
    printf("Enter power in dBm:\n> ");

    get_valid_number(&dBm, 1); // any real value allowed

    PmW = pow(10, dBm / 10);

    printf("Power = %.2f mW\n", PmW);
    wait_for_enter();
}