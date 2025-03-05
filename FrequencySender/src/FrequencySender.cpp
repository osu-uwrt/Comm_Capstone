#include "FrequencySender.hpp"
#include "math.h"

#include "chrono"
#include "iostream"

#include <iostream>
#include <fstream>

FrequencyGenerator::FrequencyGenerator(int minFreq, int maxFreq) : minFreq_(minFreq), maxFreq_(maxFreq)
{
    maxADCValue = 4096;
    minADCValue = 0;

    currentFreq_ = -1;

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
    currentFreq_ = (double)minFreq_ + frequency * (maxFreq_ - minFreq_);
}

void FrequencyGenerator::sendFrequency()
{
    int num = 0;
    std::ofstream file;
    file.open("/home/markc/OSU/ECE4905/Comm_Capstone/samples.txt");
    while (sending_)
    {
        if (currentFreq_ == -1)
        {
            uint16_t message = htons(0);

            ssize_t sent = sendto(socket_, &message, sizeof(message), 0,
                                  (sockaddr *)&serverAddr, sizeof(serverAddr));

            continue;
        }
        auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
        auto currentNanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();

        double nsAsDouble = (double)currentNanoSeconds / 1e9;

        uint16_t msg = maxADCValue / 2 * sin(currentFreq_ * nsAsDouble * 2 * M_PI) + maxADCValue / 2;

        uint16_t message = htons(maxADCValue / 2 * sin(currentFreq_ * nsAsDouble * 2 * M_PI) + maxADCValue / 2);

        ssize_t sent = sendto(socket_, &message, sizeof(message), 0,
                              (sockaddr *)&serverAddr, sizeof(serverAddr));
        if (sent < 0)
        {
            perror("Send failed");
            break;
        }

        if (num < 500000)
        {
            file << std::to_string(msg) << ",";
            file << std::to_string(nsAsDouble) << "\n";
            num++;
        }
        else if (num == 500000)
        {
            file.close();
            num++;
        }
    }
}