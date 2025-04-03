#include "message.h"

#define MESSAGE_DURATION 1000
#define PWM_CLOCK_FREQ 125000000.0
#define RESPONSE_FREQ 15000


typedef struct Transmission {
    int outputPin_;
    int outputSlice_;
    int outputChannel_;
    double dutyCycle_;
    int messageIndex_;
} Transmission_t;

typedef struct TransmissionData {
    Message_t msg;
    Transmission_t trns;
    repeating_timer_t messageTimer;
} TransmissionData_t;

void setupTransmission(Transmission_t *trns);

bool sendMessage(TransmissionData_t *data);

bool sendResponse(Transmission_t trns);

bool updateFrequency(repeating_timer_t *t);