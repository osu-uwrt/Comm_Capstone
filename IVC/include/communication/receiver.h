#ifndef RECEIVER_H
#define RECEIVER_H
#include "hardware/dma.h"

#define SAMPLES_PER_NIBBLE 1000

#include "communication/message.h"

struct Receiver {
    int inputPin_;

    // 2 Frequencies per character
    uint8_t samples[SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2];
    double filteredSamples[SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2];
    uint dmaChannel_;
    dma_channel_config dmaCfg_;
};

void setupReceiver(struct Receiver *rec, int inputPin);
void setupADC(struct Receiver *rec);
void readFromADC(struct Receiver *rec, struct Message *message, char *string);

#endif