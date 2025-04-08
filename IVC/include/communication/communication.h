#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "receiver.h"
#include "transmission.h"
#include "message.h"

enum Status
{
    STANDBY,
    SENDING,
    RECEIVING
};

typedef struct Communication
{
    Receiver_t *receiver_;
    Transmission_t *transmitter_;
    enum Status status_;
} Communication_t;

int createCommuncation(int inputPin, int outputPin);
int sendMessage(Message_t *message);

#endif // COMMUNICATION_H