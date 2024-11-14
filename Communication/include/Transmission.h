#include "pico/stdlib.h"
#include "Message.h"

struct Transmission {
    uint outputPin;
    int centerFrequency;
    double byteTimeLen;
};

bool sendMessage(struct Transmission trsm, struct Message msg);