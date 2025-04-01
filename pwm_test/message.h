#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#define MAX_MESSAGE_SIZE 4
#define MIN_FREQUENCY 15500
// This is same as SAMPLES_PER_NIBBLE 
// #define BYTE_SAMPLE_LENGTH 100

typedef struct Message {
    double frequencies[MAX_MESSAGE_SIZE * 2];
} Message_t;

int convertMessageToFrequencies(char *message, Message_t *frequencies, int messageSize);

int frequenciesToMessage(Message_t *frequencies, char *message);

#endif // MESSAGE_H