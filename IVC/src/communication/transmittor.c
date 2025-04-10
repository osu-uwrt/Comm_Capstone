#include "communication/transmittor.h"
#include <stdlib.h>
#include <stdio.h>
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

void setupTransmission(Transmission_t *trns, uint outputPin, double dutyCycle)
{
    trns->outputPin_ = outputPin;
    trns->dutyCycle_ = dutyCycle;
    // get output channel and slice
    trns->outputChannel_ = pwm_gpio_to_channel(outputPin);
    trns->outputSlice_ = pwm_gpio_to_slice_num(outputPin);

    trns->sending_ = false;

    // set up the gpio pin in transmission
    gpio_set_function(outputPin, GPIO_FUNC_PWM);

    // enable pwm for messages
    pwm_set_enabled(trns->outputSlice_, true);
}

bool sendMessage(Transmission_t *trns, Message_t *message)
{
    if (trns->sending_) {
        printf("Cannot send message: Currently sending a message\n");
        return false;
    }
    trns->sending_ = true;

    TransmissionData_t data;

    data.trns = trns;
    data.msg = message;
    trns->messageIndex_ = 0;
    printf("FREQ_DURATION: %d\n", FREQ_DURATION);
    bool slotAvailable = add_repeating_timer_us(-FREQ_DURATION, updateFrequency, (void *)&data, &(data.messageTimer));
    sleep_ms(1000);

    // print an error message if no slots are available
    if (!slotAvailable)
    {
        printf("ERROR: no alarm slots available to create a timer\n");
    }

    return slotAvailable;
}

bool sendResponse(Transmission_t *trns, bool ack)
{
    // send just one beep
    Message_t transmissionMessage;
    for (int i = 0; i < 4 + MAX_BIT_SIZE / 3; i++)
    {
        transmissionMessage.frequencies[i] = ack ? ACK_FREQ : NACK_FREQ;
    }

    return sendMessage(trns, &transmissionMessage);
}

bool updateFrequency(struct repeating_timer *t)
{
    // get the next frequency to be played in the message
    printf("Updating frequency\n");
    TransmissionData_t *data = (TransmissionData_t *)t->user_data;

    double currentFreq = convertSampleDiffToFreq(data->msg->frequencies[data->trns->messageIndex_]);
    printf("%f\n", currentFreq);
    // int period = PWM_CLOCK_FREQ / 8000.0 - 1;
    int period = PWM_CLOCK_FREQ / currentFreq - 1;

    // change the frequency of the message signal
    pwm_set_wrap(data->trns->outputSlice_, period);
    pwm_set_chan_level(data->trns->outputSlice_, data->trns->outputChannel_, data->trns->dutyCycle_ * period);

    // update the index to go to the next frequency or stop if the end is reached
    if (data->trns->messageIndex_ < 4 + MAX_BIT_SIZE / 3)
    {
        data->trns->messageIndex_++;
    }
    else
    {
        cancel_repeating_timer(t);
        pwm_set_chan_level(data->trns->outputSlice_, data->trns->outputChannel_, 0);
        data->trns->sending_ = false;
        // data->trns.messageIndex_ = 0;
    }

    return true;
}