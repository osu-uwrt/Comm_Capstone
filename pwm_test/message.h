#ifndef MESSAGE_H
#define MESSAGE_H

typedef struct Message
{
    int msgLen;
    int* frequencies;
} Message_t;

struct Message createMessage(int msgLen, int* frequencies);

void stringToMessage(char *strMsg, int msgLen, struct Message *msg);
void messageToString(char *strMsg, int msgLen, struct Message msg);

#endif