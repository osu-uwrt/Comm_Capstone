#ifndef TRANSMITTOR_H
#define TRANSMITTOR_H

typedef struct Transmittor
{
    int outputPin_;

    int dutyCycle_;
} Transmittor_t;

void setupTransmitter(Transmittor_t *trns, int outputPin, int dutyCycle);

#endif // TRANSMITTOR_H