#ifndef DSP_H
#define DSP_H

#define SAMPLE_RATE 500000.0

#include <stdint.h>

void applyIIRFilter(int *filteredSamples, uint8_t *samples_, double *b, double *a, int order, int samplesSize);
void bandpassFilter(uint8_t *samples_, int *filteredSamples_, int size);
int getSampleDifference(int *filteredSamples, int size);

#endif // DSP_H
