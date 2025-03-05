#ifndef FREQUENCY_SENDER_H
#define FREQUENCY_SENDER_H

#include "inttypes.h"
#include "thread"

#include "arpa/inet.h"

class FrequencyGenerator {
public:
    FrequencyGenerator(int minFreq, int maxFreq);
    ~FrequencyGenerator();

    void setFrequency(double frequency);

    void sendFrequency();

private:
    bool sending_;
    uint16_t maxADCValue, minADCValue;
    int minFreq_, maxFreq_, socket_;
    double currentFreq_;
    sockaddr_in serverAddr{};
    
    std::thread sendingThread;
};

#endif