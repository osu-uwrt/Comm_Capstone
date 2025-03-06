#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define MESSAGE_SLICE 0
#define MESSAGE_CHANNEL PWM_CHAN_A
#define TIMING_SLICE 1
#define MESSAGE_DURATION 100000
#define PWM_CLOCK_FREQ 125000000
#define DUTY_CYCLE 0.5
#define MESSAGE_BUFFER_LEN 10
int* messageFrequencyBuffer;
int messageIndex = 0;

void updateFrequency();

int main()
{
    stdio_init_all();

    for (int i=5; i >= 0; i--) {
        printf("countdown: %d\n", i);
        sleep_ms(1000);
    }

    messageFrequencyBuffer = (int*)malloc(MESSAGE_BUFFER_LEN * sizeof(int));
    
    //make the current list of frequencies start at 10kHz and go up by 1kHz each
    for(int i = 0; i < MESSAGE_BUFFER_LEN; i++){
        messageFrequencyBuffer[i] = (10+i) * 1000;
    }

    // use pin 0 for PWM output
    gpio_set_function(MESSAGE_SLICE * 2, GPIO_FUNC_PWM);

    //allow interrupts for the timing PWM slice
    pwm_clear_irq(TIMING_SLICE);
    pwm_set_irq_enabled(TIMING_SLICE, true);
    irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), updateFrequency);
    irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);

    //enable pwm for messages and have it start out always outputting low
    pwm_set_chan_level(MESSAGE_SLICE, MESSAGE_CHANNEL, 0);
    pwm_set_enabled(MESSAGE_SLICE, true);

    //enable pwm for timing
    pwm_set_wrap(TIMING_SLICE, MESSAGE_DURATION);
    pwm_set_enabled(TIMING_SLICE, true);


    free(messageFrequencyBuffer);


    /*while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }*/
}

void updateFrequency(){
    //clear interrupt flag
    pwm_clear_irq(TIMING_SLICE);

    //get the next frequency to be played in the message
    int currentFreq = messageFrequencyBuffer[messageIndex];
    int period = currentFreq/PWM_CLOCK_FREQ - 1;

    //change the frequency of the message signal
    pwm_set_wrap(MESSAGE_SLICE, period);
    pwm_set_chan_level(MESSAGE_SLICE, MESSAGE_CHANNEL, DUTY_CYCLE * period);


    //update the index to go to the next frequency or loop around if the end is reached
    //you can change how the end of the list is handled
    if(messageIndex < MESSAGE_BUFFER_LEN){
        messageIndex ++;
    }else{
        messageIndex = 0;
    }
}

