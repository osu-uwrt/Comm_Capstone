#ifndef RECEIVER_H
#define RECEIVER_H

#define INPUT_PIN 26

#include <hardware/adc.h>
#include <hardware/dma.h>

#include "communication/message.h"

#include "stdint.h"

typedef struct Receiver {

    // 2 Frequencies per character
    uint8_t samples[MAX_INITIAL_FREQUENCY_LEN + SAMPLES_PER_FREQUENCY * MAX_BIT_SIZE / 3];
    int filteredSamples[MAX_INITIAL_FREQUENCY_LEN + SAMPLES_PER_FREQUENCY * MAX_BIT_SIZE / 3];

    uint dmaChannel_;
    dma_channel_config dmaCfg_;
} Receiver_t;

void setupADC(Receiver_t *rec);
void readFromADC(Receiver_t *rec);
void decodeMessage(Receiver_t *rec, Message_t *message, char *string);

int detectResponse(Receiver_t *rec);

#endif // RECEIVER_H