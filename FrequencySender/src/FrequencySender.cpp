#include "FrequencySender.hpp"
#include "math.h"

#include "chrono"
#include "iostream"

FrequencyGenerator::FrequencyGenerator(int minFreq, int maxFreq)
{
    maxADCValue = 4096;
    minADCValue = 0;

    currentFreq_ = 0;

    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);                    // Port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Server address

    sending_ = true;

    sendingThread = std::thread(&FrequencyGenerator::sendFrequency, this);
}

FrequencyGenerator::~FrequencyGenerator()
{
    sending_ = false;
    if (sendingThread.joinable())
        sendingThread.join();
}

void FrequencyGenerator::setFrequency(double frequency)
{
    currentFreq_ = minFreq_ + frequency * (maxFreq_ - minFreq_);
}

void FrequencyGenerator::sendFrequency()
{
    while (sending_)
    {
        if (currentFreq_ == 0)
        {
            uint16_t message = htons(0);

            ssize_t sent = sendto(socket_, &message, sizeof(message), 0,
                                  (sockaddr *)&serverAddr, sizeof(serverAddr));

            continue;
        }
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto currentNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count() % 1000000000;

        double nsAsDouble = (double)currentNanoSeconds / 1e9;

        uint16_t message = htons(maxADCValue * sin(currentFreq_ * nsAsDouble) + maxADCValue / 2);

        ssize_t sent = sendto(socket_, &message, sizeof(message), 0,
                              (sockaddr *)&serverAddr, sizeof(serverAddr));
        if (sent < 0)
        {
            perror("Send failed");
            break;
        }
    }
}