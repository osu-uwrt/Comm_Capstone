#ifndef DSP_H
#define DSP_H
#define SAMPLE_RATE 500000.0

#include <stdint.h>

void applyIIRFilter(double *filteredSamples, uint8_t *samples_, double *b, double *a, int order, int samplesSize);
void bandpassFilter(uint8_t *samples_, double *filteredSamples_, int size);
double getFrequency(double *filteredSamples, int size);

#endif
