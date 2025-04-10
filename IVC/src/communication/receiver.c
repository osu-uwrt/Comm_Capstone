#include <pico/time.h>

#include "communication/receiver.h"
#include "tools/dsp.h"

int lookForStart(Receiver_t *rec) {
    int currentChain = 0, i = 0;
    for (i = 0; i < SAMPLES_PER_FREQUENCY * 2; i++) {
        if (rec->samples[i] <= 20) {
            currentChain++;
        } else {
            currentChain = 0;
        }
        if (currentChain > 100) {
            printf("Found end of first frequency: %d\n", i);
            i -= 99;
            break;
        }
    }
    printf("Found end of first frequency: %d\n", i);
    int potentialStart = getSampleDifference(rec->filteredSamples, i);
    printf("Got start sample diff: %d\n", potentialStart);
    if (potentialStart != INITIAL_SAMPLE_DIFFERENCE) {
        printf("Expected first start frequency %d, got %d\n", INITIAL_SAMPLE_DIFFERENCE, potentialStart);
        return -1;
    }
    i += SAMPLES_PER_FREQUENCY * 2;
    // potentialStart = getSampleDifference(&rec->filteredSamples[i], SAMPLES_PER_FREQUENCY);
    // printf("Got second sample diff: %d\n", potentialStart);
    // if (potentialStart != INITIAL_SAMPLE_DIFFERENCE) {
    //     printf("Expected second start frequency %d, got %d\n", INITIAL_SAMPLE_DIFFERENCE, potentialStart);
    //     return -2;
    // }
    // i += SAMPLES_PER_FREQUENCY * 2;
    return i;
}

void setupADC(Receiver_t *rec)
{
    adc_gpio_init(INPUT_PIN);

    adc_init();
    adc_select_input(0);
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
    channel_config_set_transfer_data_size(&rec->dmaCfg_, DMA_SIZE_8);
    channel_config_set_read_increment(&rec->dmaCfg_, false);
    channel_config_set_write_increment(&rec->dmaCfg_, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&rec->dmaCfg_, DREQ_ADC);
}
void readFromADC(Receiver_t *rec)
{
    printf("Starting capture\n");
    // uint64_t start = time_us_64();
    dma_channel_configure(rec->dmaChannel_, &rec->dmaCfg_,
                          rec->samples,                              // dst
                          &adc_hw->fifo,                             // src
                          SAMPLES_PER_FREQUENCY * MAX_BIT_SIZE / 3 + MAX_INITIAL_FREQUENCY_LEN, // transfer count
                          true                                       // start immediately
    );
    adc_run(true);

    // Once DMA finishes, stop any new conversions from starting, and clean up
    // the FIFO in case the ADC was still mid-conversion.
    dma_channel_wait_for_finish_blocking(rec->dmaChannel_);

    adc_run(false);
    adc_fifo_drain();
    // uint64_t end = time_us_64();

    // printf("Took %fms\n", (double) (end - start) / 1e3);
}

void decodeMessage(Receiver_t *rec, Message_t *message, char *string)
{
    bandpassFilter(rec->samples, rec->filteredSamples, MAX_INITIAL_FREQUENCY_LEN + SAMPLES_PER_FREQUENCY * MAX_BIT_SIZE / 3);

    // printf("Ran filter\n");
    // for (int i = 0; i < 4000; i++) {
    //     printf("%d, %d, %d\n", i, rec->samples[i], rec->filteredSamples[i]);
    // }
    int startMessagePos = lookForStart(rec);

    printf("Message starts at %d\n", startMessagePos);

    for (int i = startMessagePos; i < SAMPLES_PER_FREQUENCY * MAX_BIT_SIZE / 3 + startMessagePos; i += SAMPLES_PER_FREQUENCY)
    {
        // printf("i: %d\n", i);
        message->frequencies[(i - startMessagePos) / SAMPLES_PER_FREQUENCY] =  getSampleDifference(&rec->filteredSamples[i], SAMPLES_PER_FREQUENCY);
    }
    printf("Converting message to string\n");

    messageToString(message, string);
}

int detectResponse(Receiver_t *rec) {
    readFromADC(rec);

    int currentChain = 0, i = 0;
    for (i = 0; i < SAMPLES_PER_FREQUENCY; i++) {
        if (rec->samples[i] == 0) {
            currentChain++;
        } else {
            currentChain = 0;
        }
        if (currentChain > 100) {
            printf("Found end of first frequency: %d\n", i);
            i -= 99;
            break;
        }
    }
}