#ifndef MESSAGE_H
#define MESSAGE_H

struct Message
{
    int msgLen;
    int* frequencies;
};

struct Message createMessage(int msgLen, int* frequencies);

void stringToMessage(char *strMsg, int msgLen, struct Message *msg);
void messageToString(char *strMsg, int msgLen, struct Message msg);

#endif