#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
// Max number of bits that can be sent
#define MAX_BIT_SIZE 33
// Largest string that can be sent
#define MAX_STRING_SIZE MAX_BIT_SIZE / 8 + 2
// Number of samples expected per frequency
#define SAMPLES_PER_FREQUENCY 1000
// Initial Frequency at start of every message
#define INITIAL_SAMPLE_DIFFERENCE 32
// Sends initial frequency twice, with 0 buffer of length SAMPLES_PER_FREQUENCY
#define MAX_INITIAL_FREQUENCY_LEN SAMPLES_PER_FREQUENCY * 4

extern int freqList[];

typedef struct Message
{
    int frequencies[4 + MAX_BIT_SIZE / 3];
} Message_t;

int convertStringToMessage(const char *string, Message_t *message, int messageSize);

int messageToString(Message_t *message, char *string);

double convertSampleDiffToFreq(int sampleDiff);

#endif // MESSAGE_H