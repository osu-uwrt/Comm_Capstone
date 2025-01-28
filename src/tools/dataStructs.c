#include "tools/dataStructs.h"

#include "stdio.h"
#include "stdlib.h"

struct CircularBuffer *createBuffer(int size_)
{
    struct CircularBuffer *buffer = malloc(sizeof(struct CircularBuffer));
    buffer->buffer = malloc(size_ * sizeof(struct ADCReading));
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = 0;
    buffer->maxSize = size_;
}

int addReading(struct CircularBuffer *buffer, uint16_t reading, time_t timestamp) {
    if (buffer->size == buffer->maxSize) {
        buffer->tail = (buffer->tail + 1) % buffer->maxSize;
        buffer->size--;
    }

    buffer->buffer[buffer->head].timestamp = timestamp;
    buffer->buffer[buffer->head].reading = reading;
    buffer->head = (buffer->head + 1) % buffer->maxSize;
    buffer->size++;
}


void print_buffer(struct CircularBuffer* cb) {
    int index = cb->tail;
    for (int i = 0; i < cb->size; i++) {
        struct ADCReading* reading = &cb->buffer[index];
        printf("Timestamp: %ld, Value: %d\n",
               reading->timestamp, reading->reading);
        index = (index + 1) % cb->maxSize;
    }
}

void freeCircularBuffer(struct CircularBuffer* buffer) {
    free(buffer->buffer);
    free(buffer);
}