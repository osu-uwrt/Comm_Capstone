#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "communication/receiver.h"
#include "communication/transmittor.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 0 and 1 for UART0
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define TRANSDUCER_PIN 3

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

    gpio_init(TRANSDUCER_PIN);

    gpio_set_dir(TRANSDUCER_PIN, GPIO_OUT);
    gpio_put(TRANSDUCER_PIN, 1);

    // for (int i = 0; i < 5; i++) {
    //     printf("%d\n", i);
    //     sleep_ms(1000);
    // }

    Transmission_t transmittor;
    setupTransmission(&transmittor, OUTPUT_PIN, DUTY_CYCLE);

    Message_t message;
    convertStringToMessage("blah", &message, 4);

    sendMessage(&transmittor, &message);

    printf("Sent message\n");

    sleep_ms(10000);

    // printf("Done\n");

    // Receiver_t receiver;
    // setupADC(&receiver);

    // Message_t msg;
    // char string[MAX_STRING_SIZE];

    // // while (true)
    // // {

    // while (true)
    // {
    //     printf("Waiting for signal\n");
    //     uint16_t read = adc_read();
    //     printf("reading: %i\n", read);
    //     if (read > (uint16_t)500)
    //     {
    //         break;
    //     }
    // }
    // readFromADC(&receiver);
    // decodeMessage(&receiver, &msg, string);
    // sleep_ms(1000);

    // printf("message: %s\n", string);
    // }
}
