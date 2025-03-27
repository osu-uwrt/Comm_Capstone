#include <math.h>

#include "message.h"

int convertMessageToFrequencies(char *message, Message_t *frequencies, int messageSize)
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

int frequenciesToMessage(Message_t *frequencies, char *message)
{
    for (int i = 0; i < MAX_MESSAGE_SIZE; i++)
    {
        char currentChar = '\x00';
        unsigned int first = 0, second = 0;
        first = (unsigned int)((frequencies->frequencies[i * 2] - MIN_FREQUENCY) / 250.);
        second = (unsigned int)((frequencies->frequencies[i * 2 + 1] - MIN_FREQUENCY) / 250.);
        // printf("%d, %d\n", first, second);

        if (first > 7)
        {
            currentChar = '\x00';
        }
        else
        {
            currentChar += ((char) first << 4) + (char) second;
        }
        // printf("%c\n", currentChar);
        message[i] = currentChar;
    }
}