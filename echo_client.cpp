#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const char *SERVER_IP = "127.0.0.1";
const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    // 소켓 생성
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error: Could not create socket\n";
        return EXIT_FAILURE;
    }

    // 서버 주소 설정
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // 서버에 연결
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Connection failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server\n";

    // 메시지 전송
    char message[BUFFER_SIZE] = "Hello, server!";
    if (send(client_socket, message, strlen(message), 0) < 0) {
        std::cerr << "Error: Send failed\n";
        return EXIT_FAILURE;
    }
    std::cout << "Message sent to server\n";

    // 서버로부터 응답 받기
    char buffer[BUFFER_SIZE] = {0};
    int valread = read(client_socket, buffer, BUFFER_SIZE);
    if (valread < 0) {
        std::cerr << "Error: Read failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Message from server: " << buffer << std::endl;

    // 소켓 닫기
    close(client_socket);

    return EXIT_SUCCESS;
}

