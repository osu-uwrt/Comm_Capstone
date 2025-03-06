#include <math.h>

#include "communication/message.h"

int convertMessageToFrequencies(char *message, Message *frequencies, int messageSize)
{
    if (MAX_MESSAGE_SIZE * 2 < messageSize * 2)
    {
        printf("Message is too long for payload: Can only be %d characters long, Got %d\n", MAX_MESSAGE_SIZE, messageSize);
        return -1;
    }

    for (int i = 0; i < messageSize; i++)
    {
        frequencies->frequencies[i * 2] = ((message[i] >> 4) * 250) + MIN_FREQUENCY;
        frequencies->frequencies[i * 2 + 1] = ((message[i] & 0x0F) * 250) + MIN_FREQUENCY;
    }

    return 0;
}

int frequenciesToMessage(Message *frequencies, char *message)
{
    for (int i = 0; i < MAX_MESSAGE_SIZE; i++)
    {
        char currentChar = '\x00';
        unsigned int first = 0, second = 0;
        first = (unsigned int)((frequencies->frequencies[i * 2] - MIN_FREQUENCY) / 250.);
        second = (unsigned int)((frequencies->frequencies[i * 2 + 1] - MIN_FREQUENCY) / 250.);

        if (first > 7)
        {
            currentChar = '\x00';
        }
        else
        {
            currentChar += ((char) first << 4) + (char) second;
        }
        message[i] = currentChar;
    }
}

// void messageToSamples(struct Message *frequencies, int maxFrequencies, int samples[BYTE_SAMPLE_LENGTH * MAX_MESSAGE_SIZE * 2]) {
//     int sampleRate = 500000;
//     int phase = 0;
//     double ts = 1. / sampleRate;

//     for (int frequency = 0; frequency < MAX_MESSAGE_SIZE * 2; frequency++) {

//         for (int i = 0; i < BYTE_SAMPLE_LENGTH; i++) {
//             sample = sin(2 * M_PI * frequencies->frequencies[frequency] * (BYTE_SAMPLE_LENGTH * MAX_MESSAGE_SIZE * 2))
//         }
//     }
// }