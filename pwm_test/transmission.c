#include "transmission.h"
#include <stdlib.h>
#include <stdio.h>
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

void setupTransmission(Transmission_t trns){

    //get output channel and slice
    trns.outputChannel_ = pwm_gpio_to_channel(trns.outputPin_);
    trns.outputSlice_ = pwm_gpio_to_slice_num(trns.outputPin_);


    //set up the gpio pin in transmission
    gpio_set_function(trns.outputPin_, GPIO_FUNC_PWM);

    //enable pwm for messages
    pwm_set_enabled(trns.outputSlice_, true);
}

bool sendMessage(TransmissionData_t *data){
    
    bool slotAvailable = add_repeating_timer_us(-MESSAGE_DURATION, updateFrequency, (void*)data, &(data->messageTimer));
    
    //print an error message if no slots are available
    if(!slotAvailable){
        printf("ERROR: no alarm slots available to create a timer\n");
    }

    return slotAvailable;
    
}

void sendResponse(Transmission_t trns){

}

bool updateFrequency(struct repeating_timer *t){
    //get the next frequency to be played in the message
    TransmissionData_t *data = (TransmissionData_t*) t.user_data;

    int currentFreq = (data->msg.frequencies)[data->trns.messageIndex_];
    int period = PWM_CLOCK_FREQ/currentFreq - 1;

    //change the frequency of the message signal
    pwm_set_wrap(data->trns.outputSlice_, period);
    pwm_set_chan_level(data->trns.outputSlice_, data->trns.outputChannel_, data->trns.dutyCycle_ * period);

    //update the index to go to the next frequency or stop if the end is reached
    if(data->trns.messageIndex_ < sizeof(data->msg.frequencies) - 1){
        data->trns.messageIndex_ ++;
    }else{
        cancel_repeating_timer(t);
        pwm_set_chan_level(data->trns.outputSlice_, data->trns.outputChannel_, 0);
    }

    return true;
}