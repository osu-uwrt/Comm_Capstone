#include "message.h"

#define MESSAGE_DURATION 1000
#define PWM_CLOCK_FREQ 125000000.0

typedef struct Transmission {
    int outputPin_;

    int dutyCycle_;
} Transmission_t;


void sendMessage(Message_t *frequencies, Transmission_t transmission);

bool updateFrequency(struct repeating_timer *t);