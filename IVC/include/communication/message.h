#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#define MAX_MESSAGE_SIZE 7
#define MIN_FREQUENCY 15500
#define BYTE_SAMPLE_LENGTH 1000

typedef struct Message_ {
    double frequencies[MAX_MESSAGE_SIZE * 2];
} Message;

int convertMessageToFrequencies(char *message, Message *frequencies, int messageSize);

int frequenciesToMessage(Message *frequencies, char *message);

#endif // MESSAGE_H