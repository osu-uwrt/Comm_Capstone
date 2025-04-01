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
#define MESSAGE_BUFFER_LEN 8
volatile int messageFrequencyBuffer[MESSAGE_BUFFER_LEN] = {15625,16129,16667,17241,17857,18519,19231,20000};

//bool updateFrequency(struct repeating_timer*);

int main()
{
    stdio_init_all();

    for (int i=6; i >= 1; i--) {
        printf("countdown: %d\n", i);
        sleep_ms(1000);
    }
    
    TransmissionData_t bigboye;

    //set up the transmission with the correct numbers
    bigboye.trns.dutyCycle_ = 0.5;
    bigboye.trns.messageIndex_ = 0;
    bigboye.trns.outputChannel_ = 1;
    bigboye.trns.outputSlice_ = 3;
    bigboye.trns.outputPin_ = 7;

    //add a message to the transmission data object
    for(int i=0; i<MESSAGE_BUFFER_LEN; i++){
        bigboye.msg.frequencies[i] = messageFrequencyBuffer[i];
    }

    setupTransmission(&(bigboye.trns));

    sendMessage(&bigboye);

    while(true){
        sleep_ms(1000);
    }

}