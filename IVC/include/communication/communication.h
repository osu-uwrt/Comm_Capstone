#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "communication/receiver.h"
#include "communication/transmittor.h"
#include "communication/message.h"

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

int createCommuncation(Communication_t *radio, Receiver_t *receiver, Transmission_t *transmittor);
int sendCommMessage(Communication_t *radio, char *string, int stringSize);

#endif // COMMUNICATION_H