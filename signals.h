#ifndef SIGNALS_H
#define SIGNALS_H

// WaveformType enum used for readability
typedef enum {
    WAVE_SINE = 1,
    WAVE_SQUARE,
    WAVE_TRIANGLE
} WaveformType;

// struct used to represent a signal
typedef struct {
    double amplitude;
    double frequency;
    double phase;
    int sample_rate;
    int length;
    WaveformType type;
} Signal;

void SignalsMenu(void);

#endif