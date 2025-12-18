#include <stdio.h>
#include <math.h>
#include "signals.h"
#include "input_guard.h"

#define PI 3.141592653589793

// INTERNAL PLOTTING FUNCTIONS
static void signal_plot_menu(void);
static void generate_samples(Signal s, double *buffer);
static void plot_ascii(double *buffer, int samples);

// MAIN SIGNALS MENU
void SignalsMenu(void) {
    double choiceInput;
    int choice;

    while (1) {
        printf("\n===== Signal Tools =====\n");
        printf("1. ASCII Signal Plotter\n");
        printf("2. Back\n");
        printf("Choose 1-2:\n> ");

        if (get_valid_number(&choiceInput, 1) != 0) continue;
        if (choiceInput != (int)choiceInput) continue;

        choice = (int)choiceInput;

        switch (choice) {
            case 1: signal_plot_menu(); break;
            case 2: return;
            default: printf("Invalid choice.\n");
        }
    }
}

// SIGNAL PLOTTER MENU
static void signal_plot_menu(void) {
    Signal s;
    double temp;

    printf("\n--- ASCII Signal Plotter ---\n");

    /* Waveform selection */
    printf("Waveform type:\n");
    printf("1. Sine\n");
    printf("2. Square\n");
    printf("3. Triangle\n");
    printf("Choose 1-3:\n> ");

    get_valid_number(&temp, 1);
    if (temp != (int)temp) return;
    if (temp < 1 || temp > 3) {
        printf("Invalid waveform type.\n");
        return;
    }
    s.type = (WaveformType)((int)temp); // Using enum and structs to organise signal parameters

    // Amplitude
    printf("Enter amplitude:\n> ");
    get_valid_number(&s.amplitude, 1);

    // Number of cycles (frequency)
    printf("Enter number of cycles to show (e.g. 1-5):\n> ");
    get_valid_number(&s.frequency, 0);


    // Phase
    printf("Enter phase (radians):\n> ");
    get_valid_number(&s.phase, 1);

    // Sample rate
    printf("Enter sample rate (samples/sec):\n> ");
    get_valid_number(&temp, 0);
    s.sample_rate = (int)temp;

    /* Number of samples */
    printf("Number of samples (e.g., 100):\n> ");
    get_valid_number(&temp, 0);
    s.length = (int)temp;

    // Allocate buffer
    double buffer[s.length];

    // Generate samples
    generate_samples(s, buffer);

    // Plot ASCII
    plot_ascii(buffer, s.length);

    wait_for_enter();
}

// GENERATE SIGNAL SAMPLES
static void generate_samples(Signal s, double *buffer) {
    for (int n = 0; n < s.length; n++) {
        // Normalised time from 0 to 1 across the plot
        double t = (double)n / (double)(s.length - 1);
        // Angle over 'frequency' cycles plus phase
        double angle = 2 * PI * s.frequency * t + s.phase;

        switch (s.type) {
            case WAVE_SINE:
                buffer[n] = s.amplitude * sin(angle);
                break;

            case WAVE_SQUARE: {
                double x = sin(angle);
                buffer[n] = (x >= 0) ? s.amplitude : -s.amplitude;
                break;
            }

            case WAVE_TRIANGLE: {
                double x = fmod(s.frequency * t, 1.0); // phase kept at 0-1 in each cycle
                double d = x - 0.5;
                if (d < 0) d = -d;
                buffer[n] = s.amplitude * (4.0*d - 1.0);
                break;
            }
        }
    }
}

// ASCII PLOTTING FUNCTION
static void plot_ascii(double *buffer, int samples) {
    const int height = 20;
    double maxAmp = 0;

    // Find max amplitude for normalisation
    for (int i = 0; i < samples; i++) {
        double a = fabs(buffer[i]);
        if (a > maxAmp) maxAmp = a;
    }
    if (maxAmp < 1e-6) maxAmp = 1.0;

    // Map samples to vertical positions 0..height-1
    int y[samples];
    for (int i = 0; i < samples; i++) {
        double norm = buffer[i] / maxAmp; // -1..1
        double shifted = (norm + 1.0) / 2.0; // 0..1
        y[i] = (int)(shifted * (height - 1)); // 0..height-1
    }

    printf("\n--- ASCII Waveform Plot ---\n\n");

    // Draw from top row to bottom row
    for (int row = height - 1; row >= 0; row--) {
 
        for (int col = 0; col < samples; col++) {
            int thisY = y[col];
            int prevY;
            if (col > 0) {
                prevY = y[col - 1];
            } else {
                prevY = thisY;
            }

            int drawWave = 0;

            // Normal sample point
            if (thisY == row) {
                drawWave = 1;
            } else if (col > 0) {
                // Vertical connection between this sample and previous one
                int minY;
                int maxY;
                if (thisY < prevY) {
                    minY = thisY;
                    maxY = prevY;
                } else {
                    minY = prevY;
                    maxY = thisY;
                }
                if (row > minY && row < maxY) {
                    drawWave = 1;
                }
                // Horizontal connection case (flat line)
                if (row >= minY && row <= maxY && (maxY - minY) > 1) {
                    drawWave = 1;
                }
            }

            if (drawWave) {
                printf("%c", '*');
            } else if (row == height / 2) {
                printf("%c", '-'); // centre axis
            } else {
                printf("%c",' ');
            }
        }
        printf("%c", '\n');
    }
}