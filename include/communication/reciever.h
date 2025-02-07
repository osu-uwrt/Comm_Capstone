#ifndef RECIEVER_H
#define RECIEVER_H

#include "communication/message.h"

struct Reciever {
    int inputPin;
    double byteTimeLen;
};


int readMessage(struct Message *msg);

#endif