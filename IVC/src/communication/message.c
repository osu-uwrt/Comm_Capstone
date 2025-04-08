#include <math.h>

#include "communication/message.h"

int freqList[8] = {32, 31, 30, 29, 28, 27, 26, 25};

int convertStringToMessage(const char *string, Message_t *message, int messageSize)
{
    if (MAX_BIT_SIZE < messageSize * 8)
    {
        printf("Message is too long for payload: Can only be %d characters long, Got %d\n", MAX_BIT_SIZE / 8, messageSize);
        return -1;
    }

    unsigned long packed = 0;
    int bit_count = 0;
    int index = 0;

    for (int i = 0; i < messageSize; i++)
    {
        packed = (packed << 8) | string[i]; // Pack character
        bit_count += 8;

        while (bit_count >= 3)
        {                                                  // Extract 3-bit chunks
            int value = (packed >> (bit_count - 3)) & 0x7; // Get top 3 bits
            printf("%d\n", value);
            message->frequencies[index++] = freqList[value]; // Map to frequency
            bit_count -= 3;
        }
    }

    // Handle leftover bits if any
    if (bit_count > 0)
    {
        int value = (packed & ((1 << bit_count) - 1)) << (3 - bit_count);
        message->frequencies[index++] = freqList[value & 0x7];
    }

    return 0;
}

// Function to find the index of a frequency
int find_index(int freq)
{
    for (int i = 0; i < 8; i++)
    {
        if (freqList[i] == freq)
        {
            return i;
        }
    }
    return -1; // Error case (shouldn't happen with valid input)
}

int messageToString(Message_t *message, char *string)
{
    unsigned long packed = 0;
    int bit_count = 0;
    int char_index = 0;

    for (int i = 0; i < MAX_BIT_SIZE / 3; i++)
    {
        int value = find_index(message->frequencies[i]); // Get 3-bit value
        if (value == -1)
        {
            printf("Invalid %d\n", message->frequencies[i]);
            // string[char_index++] = (0 >> (bit_count - 8)) & 0xFF;
            break; // Skip invalid values
        }

        packed = (packed << 3) | value; // Append 3 bits
        bit_count += 3;

        while (bit_count >= 8)
        { // Extract characters
            string[char_index++] = (packed >> (bit_count - 8)) & 0xFF;
            printf("%c\n", string[char_index - 1]);
            bit_count -= 8;
        }
    }

    string[++char_index] = '\0'; // Null-terminate the string
}

double convertSampleDiffToFreq(int sampleDiff)
{
    if (sampleDiff == 0)
    {
        return 0;
    }
    return 1. / ((1. / 500000) * sampleDiff);
}