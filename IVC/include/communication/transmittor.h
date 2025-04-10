#include "communication/message.h"

#include "pico/time.h"

#define FREQ_DURATION 2 * SAMPLES_PER_FREQUENCY
#define PWM_CLOCK_FREQ 125000000.0
#define ACK_FREQ freqList[0]
#define NACK_FREQ freqList[8]
// #define RESPONSE_FREQ 15000

#define OUTPUT_PIN 7
#define DUTY_CYCLE 0.5

typedef struct Transmission
{
    int outputPin_;
    int outputSlice_;
    int outputChannel_;
    double dutyCycle_;
    int messageIndex_;

    bool sending_;
} Transmission_t;

typedef struct TransmissionData
{
    Message_t *msg;
    Transmission_t *trns;
    repeating_timer_t messageTimer;
} TransmissionData_t;

void setupTransmission(Transmission_t *trns, uint outputPin, double dutyCycle);
void createTrnsData(Message_t *message);

bool sendMessage(Transmission_t *trns, Message_t *message);
bool sendResponse(Transmission_t *trns, bool ack);

bool updateFrequency(repeating_timer_t *t);