#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#define MAX_MESSAGE_SIZE 20
#define MIN_FREQUENCY 15500
#define BYTE_SAMPLE_LENGTH 1000

struct Message {
    double frequencies[MAX_MESSAGE_SIZE * 2];
};

int convertMessageToFrequencies(char *message, struct Message *frequencies, int messageSize);

int frequenciesToMessage(struct Message *frequencies, char *message);

#endif