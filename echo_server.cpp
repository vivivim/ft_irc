#include <iostream>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    // 소켓 생성
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error: Could not create socket\n";
        return EXIT_FAILURE;
    }

    // 소켓 주소 설정
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Bind failed\n";
        return EXIT_FAILURE;
    }

    // 클라이언트 연결 대기
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error: Listen failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "Echo server is listening on port " << PORT << std::endl;

    // 클라이언트 요청 처리
    int client_socket;
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    while (true) {
        // 클라이언트 연결 수락
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket < 0) {
            std::cerr << "Error: Accept failed\n";
            return EXIT_FAILURE;
        }

        // 클라이언트로부터 메시지 수신 및 에코
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread < 0) {
            std::cerr << "Error: Read failed\n";
            return EXIT_FAILURE;
        }

        std::cout << "Received message from client: " << buffer << std::endl;

        // 클라이언트에게 메시지 에코
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            std::cerr << "Error: Send failed\n";
            return EXIT_FAILURE;
        }

        // 클라이언트 소켓 닫기
        close(client_socket);
    }

    // 서버 소켓 닫기
    close(server_socket);

    return EXIT_SUCCESS;
}

