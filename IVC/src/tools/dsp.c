#include <math.h>

#include "tools/dsp.h"

void applyIIRFilter(double *filteredSamples, uint8_t *samples_, double *b, double *a, int order, int samplesSize)
{
    for (int i = 0; i < samplesSize; i++)
    {
        filteredSamples[i] = b[0] * samples_[i];
        for (int j = 1; j < order + 1; j++)
        {
            if (i - j >= 0)
            {
                filteredSamples[i] += b[j] * samples_[i - j];
            }
        }
        for (int j = 1; j < order + 1; j++)
        {
            if (i - j >= 0)
            {
                filteredSamples[i] -= a[j] * filteredSamples[i - j];
            }
        }
    }
}

void bandpassFilter(uint8_t *samples_, double *filteredSamples_, int size)
{
    double b[5] = {0.0018, 0, -0.0036, 0, 0.0018};
    double a[5] = {1.0000, -3.7861, 5.4626, -3.5575, 0.8830};
    applyIIRFilter(filteredSamples_, samples_, b, a, 4, size);
}

double getFrequency(double *filteredSamples, int size)
{
    int lastPeak = -1;
    double totalFrequency = 0;
    double numPeaks = 0;
    for (int i = 5; i < size; i++)
    {
        if (filteredSamples[i] > 100 && fabs(filteredSamples[i] - filteredSamples[i - 1]) < 5)
        {
            if (i - lastPeak > 10)
            {
                if (lastPeak != -1)
                {
                    double detectedFrequency = SAMPLE_RATE / (i - lastPeak);
                    if (detectedFrequency > 14700 && detectedFrequency < 20900)
                    {
                        totalFrequency += detectedFrequency;
                        numPeaks++;
                    }
                }
                lastPeak = i;
            }
        }
    }
    if (numPeaks > 0)
    {
        double frequency = totalFrequency / numPeaks;
        if (frequency > 20200 || frequency < 14800)
        {
            return 0;
        }
        return frequency;
    }
    else
    {
        return 0;
    }
}