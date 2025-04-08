#include <math.h>
#include <stdio.h>

#include "tools/dsp.h"

void applyIIRFilter(int *filteredSamples, uint8_t *samples_, double *b, double *a, int order, int samplesSize)
{
    double previous[order];
    for (int i = 0; i < samplesSize; i++)
    {
        double filteredSample = b[0] * samples_[i];
        for (int j = 1; j < order + 1; j++)
        {
            if (i - j >= 0)
            {
                filteredSample += b[j] * samples_[i - j];
            }
        }
        for (int j = 1; j < order + 1; j++)
        {
            if (i - j >= 0)
            {
                filteredSample -= a[j] * previous[order - j];
            }
        }
        if (i < order)
        {
            previous[i] = filteredSample;
        }
        else
        {
            for (int j = 0; j < order - 1; j++)
            {
                previous[j] = previous[j + 1];
            }
            previous[order - 1] = filteredSample;
        }
        filteredSamples[i] = (int)filteredSample;
    }
}

void bandpassFilter(uint8_t *samples_, int *filteredSamples_, int size)
{
    double b[5] = {0.0018,       0, -0.0036,       0, 0.0018};
    double a[5] = {1.0000, -3.7861,  5.4626, -3.5575, 0.8830};
    applyIIRFilter(filteredSamples_, samples_, b, a, 4, size);
}

int getSampleDifference(int *filteredSamples, int size)
{
    int lastPeak = -1;
    double totalFrequency = 0;
    double differences = 0;
    int numPeaks = 0;
    int bufferSize = 20;
    for (int i = bufferSize; i < size - bufferSize; i++)
    {
        if (filteredSamples[i] > 0 && fabs(filteredSamples[i] - filteredSamples[i - 1]) < 2)
        {
            if (lastPeak != -1)
            {
                int sampleDifference = i - lastPeak;
                if (sampleDifference > 23 && sampleDifference < 35)
                {
                    differences += sampleDifference;
                    numPeaks++;
                    // printf("%d\n", sampleDifference);
                }
            }
            lastPeak = i;
            i += 10;
        }
    }
    if (numPeaks > 0)
    {
        double avgPeak = differences / numPeaks; 
        // return (int)(avgPeak < 0 ? (avgPeak - 0.5) : (avgPeak + 0.5));
        printf("Avg Peaks: %f, Num Peaks: %d\n", avgPeak, numPeaks);
        return (int)(avgPeak < 0 ? (avgPeak - 0.5) : (avgPeak + 0.5));
    }
    else
    {
        printf("No peaks\n");
        return 0;
    }
}