#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "input_guard.h"
#include "comms.h"

#define PI 3.141592653589793

// Internal functions 
static void signal_strength(void);
static void constellation_menu(void);

// MAIN COMMS MENU
void CommsMenu(void) {
    double choiceInput;
    int choice;

    while (1) {
        printf("\n===== Communications Tools =====\n");
        printf("1. Signal Strength Calculator\n");
        printf("2. Constellation Diagram (ASCII)\n");
        printf("3. Back\n");
        printf("Choose 1-3:\n> ");

        if (get_valid_number(&choiceInput, 1) != 0) continue;
        if (choiceInput != (int)choiceInput) continue;
        choice = (int)choiceInput;

        switch (choice) {
            case 1: signal_strength(); break;
            case 2: constellation_menu(); break;
            case 3: return;
            default: printf("Invalid choice.\n");
        }
    }
}

// SIGNAL STRENGTH CALCULATOR
static void signal_strength(void) {
    double Pt, Gt, Gr, Lfs, Lmisc;

    printf("\n--- Signal Strength Calculator ---\n");
    printf("Enter Transmitted Power (dBm):\n> ");
    get_valid_number(&Pt, 1);

    printf("Enter Transmitted Antenna Gain (dB):\n> ");
    get_valid_number(&Gt, 1);

    printf("Enter Received Antenna Gain (dB):\n> ");
    get_valid_number(&Gr, 1);

    printf("Enter Free Space Path Loss (dB):\n> ");
    get_valid_number(&Lfs, 0);

    printf("Enter Miscellaneous Losses (dB):\n> ");
    get_valid_number(&Lmisc, 0);

    double Pr_dBm = Pt + Gt + Gr - Lfs - Lmisc;
    double Pr_mW = pow(10.0, Pr_dBm / 10.0);

    printf("\n--- Results ---\n");
    printf("Received Power = %.2f dBm\n", Pr_dBm);
    printf("Received Power = %.3e mW\n", Pr_mW);

    wait_for_enter();
}

// NOISE SIMULATION FUNCTION
static double simulate_noise(double stddev) {
    double sum = 0.0;
    for (int i = 0; i < 12; i++)
        sum += (rand() / (double)RAND_MAX);
    return stddev * (sum - 6.0);
}


// CONSTELLATION DIAGRAM MENU
static void constellation_menu(void) {
    double temp;
    int M;

    printf("\n--- Advanced Constellation Diagram ---\n");
    printf("Supported: 4-QAM, 16-QAM, 64-QAM, 256-QAM\n");
    printf("Enter modulation order (4, 16, 64, 256):\n> ");
    get_valid_number(&temp, 0);
    M = (int)temp;

    if (M != 4 && M != 16 && M != 64 && M != 256) {
        printf("Unsupported modulation.\n");
        wait_for_enter();
        return;
    }

    // USER PARAMETERS
    int numSymbols, SNRdB, rotationDeg, fadingOption;

    printf("Number of transmitted symbols (e.g. 100-2000):\n> ");
    get_valid_number(&temp, 0);
    numSymbols = (int)temp;

    printf("SNR level in dB (0-40 recommended):\n> ");
    get_valid_number(&temp, 0);
    SNRdB = (int)temp;

    printf("Channel rotation in degrees (0-45 typical):\n> ");
    get_valid_number(&temp, 1);
    rotationDeg = (int)temp;

    printf("Use Rayleigh fading? 0 = No, 1 = Yes:\n> ");
    get_valid_number(&temp, 1);
    fadingOption = (int)temp;

    // NOISE STANDARD DEVIATION CALCULATION
    double SNRlinear = pow(10.0, SNRdB / 10.0);
    double noiseStd = sqrt(1.0 / (2.0 * SNRlinear));

    // ANGLE OF ROTATION
    double rot = rotationDeg * PI / 180.0;

    // GRID SIZE
    const int GRID = 25;
    char plot[GRID][GRID];

    for (int y = 0; y < GRID; y++)
        for (int x = 0; x < GRID; x++)
            plot[y][x] = ' ';

    int origin = GRID / 2;  

    // SELECTING THE AXIS
    int lowOrder = (M == 4 || M == 16);

    if (lowOrder) {
        // Style A: Simple crosshair
        for (int i = 0; i < GRID; i++) {
            plot[origin][i] = '-'; // horizontal axis
            plot[i][origin] = '|'; // vertical axis
        }
        plot[origin][origin] = '+';
    } else {
        // Style B: Grid with ticks
        for (int x = 0; x < GRID; x++)
            plot[origin][x] = '-';
        for (int y = 0; y < GRID; y++)
            plot[y][origin] = '|';
        plot[origin][origin] = '+';

        // Add ticks every 5 units
        for (int t = 0; t < GRID; t += 5) {
            plot[origin][t] = '+';
            plot[t][origin] = '+';
        }
    }

    // IDEAL SYMBOL LOCATIONS
    int dim = (int)sqrt(M);
    double spacing;
    if (M == 4) spacing = 2.0;
    else if (M == 16) spacing = 2.0;
    else if (M == 64) spacing = 2.0;
    else spacing = 1.0; // 256-QAM

    double axisScale = (GRID - 3) / (2.0 * (dim - 1) * spacing); // Scale for mapping I/Q to grid


    double Iideal[M], Qideal[M];
    int idx = 0;

    for (int row = 0; row < dim; row++) {
        for (int col = 0; col < dim; col++) {
            Iideal[idx] = (col - (dim - 1)/2.0) * spacing;
            Qideal[idx] = (row - (dim - 1)/2.0) * spacing;
            idx++;
        }
    }

    // FOR LOOP FOR PLOTTING IDEAL SYMBOLS (+)
    for (int k = 0; k < M; k++) {
        int x = origin + (int)(Iideal[k] * axisScale);
        int y = origin - (int)(Qideal[k] * axisScale);
        if (x >= 0 && x < GRID && y >= 0 && y < GRID)
            if (plot[y][x] == ' ') {
                if (lowOrder) {
                    plot[y][x] = '+';
                } else {
                    plot[y][x] = '.';
                }
            }
    }

    // SIMULATING TRANSMITTED SYMBOLS WITH NOISE AND FADING
    for (int n = 0; n < numSymbols; n++) {
        int pick = rand() % M;
        double I = Iideal[pick];
        double Q = Qideal[pick];

        // APPLY RAYLEIGH FADING IF SELECTED
        double h = 1.0;
        if (fadingOption == 1) {
            double u = rand() / (double)RAND_MAX;
            h = sqrt(-2.0 * log(u));
            I *= h;
            Q *= h;
        }

        // ADD NOISE
        I += simulate_noise(noiseStd);
        Q += simulate_noise(noiseStd);

        // APPLY ROTATION
        double Irot = I*cos(rot) - Q*sin(rot); // Rotation in the in-phase plane
        double Qrot = I*sin(rot) + Q*cos(rot); // Rotation in the quadrature plane

        // MAP TO GRID
        int x = origin + (int)(Irot * axisScale); // X axis
        int y = origin - (int)(Qrot * axisScale); // Y axis 

        // KEEPS ORIGIN STABLE
        if (x == origin) x++;
        if (y == origin) y--;

        if (x >= 0 && x < GRID && y >= 0 && y < GRID)
            if (plot[y][x] == ' ')
                plot[y][x] = '*';
    }

    // DISPLAYING THE CONSTELLATION DIAGRAM
    printf("\n--- %d-QAM Constellation Diagram ---\n\n", M);

    for (int y = 0; y < GRID; y++) {
        for (int x = 0; x < GRID; x++)
            printf("%c", plot[y][x]);
        printf("\n");
    }

    // LEGEND
    printf("\nLegend:\n");
    printf(" . Ideal symbol (no noise)\n");
    printf(" * Received symbol (noisy)\n");
    printf("Noise simulation model based on SNR value\n");
    printf("Rotation applies channel phase shift\n");
    printf("Rayleigh fading included\n");

    wait_for_enter();
}