#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "communication/message.h"

struct Transmission {
    int outputPin;
    double byteTimeLen;
};

int sendMessage(struct Transmission trsm, struct Message msg);

#endif 