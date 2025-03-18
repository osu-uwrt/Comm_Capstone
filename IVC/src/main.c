#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "communication/message.h"

#define SAMPLE_RATE 500000.0

void applyIIRFilter(double *filteredSamples, int *samples_, double *b, double *a, int order, int samplesSize)
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

void bandpassFilter(int *samples_, double *filteredSamples_, int size)
{
    double b[5] = {0.0018, 0, -0.0036, 0, 0.0018};
    double a[5] = {1.0000, -3.7861, 5.4626, -3.5575, 0.8830};

    applyIIRFilter(filteredSamples_, samples_, b, a, 4, size);
}

double getFrequency(double *samples, int size)
{
    int lastPeak = -1;
    double totalFrequency = 0;
    double numPeaks = 0;
    for (int i = 5; i < size; i++)
    {
        if (samples[i] > 100 && fabs(samples[i] - samples[i - 1]) < 5)
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

double random_double()
{
    return 4.0 * ((double)rand() / RAND_MAX) - 2.0;
}

int main(int argc, char *argv[])
{
    // for (int freqencies = 13000; freqencies <= 22000; freqencies += 250)
    // {
    //     double ts = 1. / 500000.;
    //     double totalTime = 0.005;
    //     int numSamples = (int)(totalTime / ts) + 1;

    //     double samples[numSamples];
    //     int x[numSamples];
    //     double y[numSamples];

    //     for (int i = 0; i < numSamples; i++)
    //     {
    //         samples[i] = ts * i;
    //     }

    //     for (int i = 0; i < numSamples; i++)
    //     {
    //         double sample = sin(2 * M_PI * freqencies * samples[i]) * 5 + random_double();
    //         // double sample = random_double();
    //         if (sample > 3.3)
    //         {
    //             sample = 3.3;
    //         }
    //         else if (sample < 0)
    //         {
    //             sample = 0.0;
    //         }
    //         x[i] = sample * (255. / 3.3);
    //     }

    //     bandpassFilter(x, y, numSamples);

    //     printf("desired frequency: %d, frequency: %f\n", freqencies, getFrequency(y, numSamples));
    //     // FILE *fp;
    //     // fp = fopen("filtered.csv", "w");
    //     // for (int i = 0; i < numSamples; i++) {
    //     //     fprintf(fp, "%d, %f, %d\n", i, y[i], x[i]);
    //     // }
    //     // fclose(fp);
    // }

    struct Message message;

    char *string = "hello world";

    convertMessageToFrequencies(string, &message, 11);

    char decoded[MAX_MESSAGE_SIZE];
    frequenciesToMessage(&message, decoded);

    printf("decoded: %s\n", decoded);

    return 0;
}