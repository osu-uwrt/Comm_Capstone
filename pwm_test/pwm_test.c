#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "message.h"
#include "transmission.h"

//use pin 7 for message outputs
#define MESSAGE_SLICE 3
#define MESSAGE_CHANNEL 1
#define MESSAGE_PIN 7
#define MESSAGE_DURATION 1000
#define PWM_CLOCK_FREQ 125000000.0
#define DUTY_CYCLE 0.5
#define MESSAGE_BUFFER_LEN 11
volatile int messageFrequencyBuffer[MESSAGE_BUFFER_LEN];
volatile int messageIndex = 0;

//bool updateFrequency(struct repeating_timer*);

int main()
{
    stdio_init_all();

    for (int i=6; i >= 1; i--) {
        printf("countdown: %d\n", i);
        sleep_ms(1000);
    }
    
    //make the current list of frequencies start at 10kHz and go up by 1kHz each
    for(int i = 0; i < MESSAGE_BUFFER_LEN; i++){
        messageFrequencyBuffer[i] = (10+i) * 1000;
    }

    // use pin 7 for PWM output
    gpio_set_function(MESSAGE_PIN, GPIO_FUNC_PWM);

    //enable pwm for messages
    pwm_set_enabled(MESSAGE_SLICE, true);

    //repeat the frequency updates
    struct repeating_timer messageTimer;
    add_repeating_timer_us(-MESSAGE_DURATION,updateFrequency,NULL,&messageTimer);

    while(true){
        sleep_ms(20);
    }

}

void sendMessage(Message_t *frequencies, int messageSize){
    

}

bool updateFrequency(struct repeating_timer *t){

    //printf("hello\n");


    //get the next frequency to be played in the message
    int currentFreq = messageFrequencyBuffer[messageIndex];
    int period = PWM_CLOCK_FREQ/currentFreq - 1;

    //printf("%d, %d, %d\n",messageIndex, currentFreq, period);


    //change the frequency of the message signal
    pwm_set_wrap(MESSAGE_SLICE, period);
    pwm_set_chan_level(MESSAGE_SLICE, MESSAGE_CHANNEL, DUTY_CYCLE * period);


    //update the index to go to the next frequency or stop if the end is reached
    if(messageIndex < MESSAGE_BUFFER_LEN - 1){
        messageIndex ++;
    }else{
        //messageIndex = 0;
        cancel_repeating_timer(t);
        pwm_set_chan_level(MESSAGE_SLICE, MESSAGE_CHANNEL, 0);
    }

    return true;
}

