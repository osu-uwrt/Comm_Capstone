#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "communication/receiver.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 0 and 1 for UART0
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main()
{
    stdio_init_all();
    // stdio_usb_init();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    Receiver_t receiver;
    setupReceiver(&receiver, 28);
    setupADC(&receiver);
    Message msg;
    char string[MAX_MESSAGE_SIZE + 1];

    while (true) {
        readFromADC(&receiver, &msg, string);
    }

    // while (true)
    // {
    //     printf("Waiting for signal\n");
    //     uint16_t read = adc_read();
    //     printf("reading: %i\n", read);
    //     if (read > (uint16_t) 3000)
    //     {
    //         break;
    //     }
    // }
    // // while (true)
    // printf("Got signal\n");
    // // for (int i = 0; i < 5; i++) {
    // //     printf("%d\n", i);
    // //     sleep_ms(1e3);
    // // }

    // Message msg;
    // char string[MAX_MESSAGE_SIZE + 1];
    // readFromADC(&receiver, &msg, string);

    // string[MAX_MESSAGE_SIZE] = '\00';

    // printf("message: %s\n", string);
}
