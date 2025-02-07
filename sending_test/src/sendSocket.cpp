#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <chrono>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Port number
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Server address

    auto startTime = std::chrono::high_resolution_clock::now();
    for (uint32_t i = 0; i < 500000; i++) {
        uint32_t message = htonl(256);
        ssize_t sent = sendto(sockfd, &message, sizeof(message), 0,
                              (sockaddr*)&serverAddr, sizeof(serverAddr));
        if (sent < 0) {
            perror("Send failed");
            break;
        }
    }

    printf("Took %fs\n", (float) std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count() / 1e9);

    close(sockfd);
    return 0;
}