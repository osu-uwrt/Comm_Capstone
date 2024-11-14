struct Message {
    int msgLen;
    int *frequencies;
};

void stringToMessage(char *strMsg, int strLen, Message *msg);