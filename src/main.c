#include "tools/dataStructs.h"

int main(int argc, char *argv[])
{
    struct CircularBuffer *buffer = createBuffer(5);
    for (int i = 0; i < 9; i++)
    {
        addReading(buffer, i, time(NULL));
    }
    print_buffer(buffer);
    freeCircularBuffer(buffer);
    return 0;
}