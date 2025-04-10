#include "communication/communication.h"

int createCommuncation(Communication_t *radio, Receiver_t *receiver, Transmission_t *transmittor)
{
    setupADC(receiver);
    setupTransmission(transmittor, OUTPUT_PIN, DUTY_CYCLE);

    radio->receiver_ = receiver;
    radio->transmitter_ = transmittor;

    radio->status_ = STANDBY;
}

int sendCommMessage(Communication_t *radio, char *string, int stringSize)
{
    if (radio->status_ != STANDBY)
    {
        printf("Cannot send message: Not on standby\n");
        return -1;
    }
    bool repeat = true;
    while (repeat)
    {
        radio->status_ = SENDING;
        Message_t message;
        int res = convertStringToMessage(string, &message, stringSize);
        if (res != 0)
        {
            printf("Could not create message\n");
            radio->status_ = STANDBY;
            return res;
        }

        res = sendMessage(radio->transmitter_, &message);
        if (!res)
        {
            printf("Failed to send message\n");
            radio->status_ = STANDBY;
            return res;
        }

        // Wait until entire message is sent
        while (radio->transmitter_->sending_)
        {
            sleep_ms(1);
        }

        res = detectResponse(radio->receiver_);

        if (res == -1)
        {
            printf("Failed to read response\n");
            radio->status_ = STANDBY;
            return res;
        }

        if (res == 1)
        {
            printf("Got ack\n");
            repeat = false;
        }
        else
        {
            printf("Got nack, resending message\n");
        }
    }

    radio->status_ = STANDBY;
}