#ifndef RECEIVER_H
#define RECEIVER_H

#include "hardware/dma.h"

#define SAMPLES_PER_NIBBLE 100

#include "communication/message.h"

typedef struct Receiver {
    int inputPin_;

    // 2 Frequencies per character
    uint8_t samples[SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2];
    double filteredSamples[SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2];

    uint dmaChannel_;
    dma_channel_config dmaCfg_;
} Receiver_t;

void setupReceiver(Receiver_t *rec, int inputPin);
void setupADC(Receiver_t *rec);
void readFromADC(Receiver_t *rec, Message *message, char *string);

#endif // RECEIVER_H