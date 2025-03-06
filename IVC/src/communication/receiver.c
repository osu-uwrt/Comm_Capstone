#include <hardware/adc.h>
#include <hardware/dma.h>
#include <pico/time.h>

#include "communication/receiver.h"
#include "tools/dsp.h"

void setupReceiver(Receiver_t *rec, int inputPin)
{
    rec->inputPin_ = inputPin;
}
void setupADC(Receiver_t *rec)
{
    adc_gpio_init(rec->inputPin_);

    adc_init();
    adc_select_input(2);
    adc_fifo_setup(
        true,  // Write each completed conversion to the sample FIFO
        true,  // Enable DMA data request (DREQ)
        1,     // DREQ (and IRQ) asserted when at least 1 sample present
        false, // We won't see the ERR bit because of 8 bit reads; disable.
        true   // Shift each sample to 8 bits when pushing to FIFO
    );

    // Divisor of 0 -> full speed. Free-running capture with the divider is
    // equivalent to pressing the ADC_CS_START_ONCE button once per `div + 1`
    // cycles (div not necessarily an integer). Each conversion takes 96
    // cycles, so in general you want a divider of 0 (hold down the button
    // continuously) or > 95 (take samples less frequently than 96 cycle
    // intervals). This is all timed by the 48 MHz ADC clock.
    adc_set_clkdiv(0);

    printf("Arming DMA\n");
    sleep_ms(1000);

    // Set up the DMA to start transferring data as soon as it appears in FIFO
    rec->dmaChannel_ = dma_claim_unused_channel(true);
    rec->dmaCfg_ = dma_channel_get_default_config(rec->dmaChannel_);

    // Reading from constant address, writing to incrementing byte addresses
    channel_config_set_transfer_data_size(&rec->dmaCfg_, DMA_SIZE_16);
    channel_config_set_read_increment(&rec->dmaCfg_, false);
    channel_config_set_write_increment(&rec->dmaCfg_, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&rec->dmaCfg_, DREQ_ADC);
}
void readFromADC(Receiver_t *rec, Message *message, char *string)
{
    printf("Starting capture\n");
    dma_channel_configure(rec->dmaChannel_, &rec->dmaCfg_,
                          rec->samples,                              // dst
                          &adc_hw->fifo,                             // src
                          SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2, // transfer count
                          true                                       // start immediately
    );
    adc_run(true);

    // Once DMA finishes, stop any new conversions from starting, and clean up
    // the FIFO in case the ADC was still mid-conversion.
    dma_channel_wait_for_finish_blocking(rec->dmaChannel_);

    adc_run(false);
    adc_fifo_drain();

    bandpassFilter(rec->samples, rec->filteredSamples, SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2);

    for (int i = 0; i < SAMPLES_PER_NIBBLE * MAX_MESSAGE_SIZE * 2; i += SAMPLES_PER_NIBBLE)
    {
        message->frequencies[i / SAMPLES_PER_NIBBLE] = getFrequency(rec->filteredSamples + (i * SAMPLES_PER_NIBBLE), SAMPLES_PER_NIBBLE);
    }

    for (int i = 0; i < 40; i++) {
        printf("Frequency: %f\n", message->frequencies[i]);
    }

    frequenciesToMessage(message, string);
}