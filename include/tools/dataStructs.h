#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <time.h>
#include <stdint.h>

typedef struct ADCReading {
    time_t timestamp;
    uint16_t reading;
};

typedef struct CircularBuffer {
    struct ADCReading *buffer;
    int head, tail, size, maxSize;
};

struct CircularBuffer* createBuffer(int size_);
int addReading(struct CircularBuffer* buffer, uint16_t reading, time_t timestamp);
void print_buffer(struct CircularBuffer* cb);

void freeCircularBuffer(struct CircularBuffer* buffer);
#endif