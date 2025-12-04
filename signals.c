#include <stdio.h>
#include <math.h>
#include "signals.h"
#include "input_guard.h"

#define PI 3.141592653589793

/* Internal functions */
static void signal_plot_menu(void);
static void generate_samples(Signal s, double *buffer);
static void plot_ascii(double *buffer, int samples);

/* ======================== MAIN SIGNAL MENU ======================== */
void SignalsMenu(void) {
    double choiceInput;
    int choice;

    while (1) {
        printf("\n===== Signal Tools =====\n");
        printf("1. ASCII Signal Plotter\n");
        printf("2. Back\n");
        printf("Choose 1–2:\n> ");

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

/* ======================== SIGNAL PLOTTER MENU ======================== */
static void signal_plot_menu(void) {
    Signal s;
    double temp;

    printf("\n--- ASCII Signal Plotter ---\n");

    /* Waveform selection */
    printf("Waveform type:\n");
    printf("1. Sine\n");
    printf("2. Square\n");
    printf("3. Triangle\n");
    printf("Choose 1–3:\n> ");

    get_valid_number(&temp, 1);
    if (temp != (int)temp) return;
    s.type = (WaveformType)((int)temp);

    /* Amplitude */
    printf("Enter amplitude:\n> ");
    get_valid_number(&s.amplitude, 1);

    /* Frequency */
    printf("Enter frequency (Hz):\n> ");
    get_valid_number(&s.frequency, 0);

    /* Phase */
    printf("Enter phase (radians):\n> ");
    get_valid_number(&s.phase, 1);

    /* Sample rate */
    printf("Enter sample rate (samples/sec):\n> ");
    get_valid_number(&temp, 0);
    s.sample_rate = (int)temp;

    /* Number of samples */
    printf("Number of samples (e.g., 100):\n> ");
    get_valid_number(&temp, 0);
    s.length = (int)temp;

    /* Allocate buffer */
    double buffer[s.length];

    /* Generate samples */
    generate_samples(s, buffer);

    /* Plot ASCII graph */
    plot_ascii(buffer, s.length);

    wait_for_enter();
}

/* ======================== GENERATE SAMPLES ======================== */
static void generate_samples(Signal s, double *buffer) {
    for (int n = 0; n < s.length; n++) {
        double t = (double)n / s.sample_rate;

        switch (s.type) {
            case WAVE_SINE:
                buffer[n] = s.amplitude * sin(2 * PI * s.frequency * t + s.phase);
                break;

            case WAVE_SQUARE: {
                double x = sin(2 * PI * s.frequency * t + s.phase);
                buffer[n] = (x >= 0) ? s.amplitude : -s.amplitude;
                break;
            }

            case WAVE_TRIANGLE: {
                double x = fmod(s.frequency * t, 1.0);
                buffer[n] = s.amplitude * (4.0 * fabs(x - 0.5) - 1.0);
                break;
            }
        }
    }
}

/* ======================== ASCII PLOTTER ======================== */
static void plot_ascii(double *buffer, int samples) {
    printf("\n--- ASCII Waveform Plot ---\n\n");

    for (int i = 0; i < samples; i++) {
        /* Scale vertically between 0–60 cols */
        int col = (int)(buffer[i] * 10 + 30);

        if (col < 0) col = 0;
        if (col > 60) col = 60;

        for (int j = 0; j < col; j++) printf(" ");
        printf("*\n");
    }
}
