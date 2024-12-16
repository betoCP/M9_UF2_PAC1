#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // Necesario para inet_pton()
#pragma comment(lib, "ws2_32.lib") // Librería de sockets para Windows

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[1024] = { 0 };
    const char* message = "¡Hola servidor!";

    // 1. Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock.\n";
        return 1;
    }

    // 2. Crear socket del cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket.\n";
        WSACleanup();
        return 1;
    }

    // 3. Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090); // Puerto 9090

    // Usar inet_pton para convertir la IP
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Error al convertir la dirección IP.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 4. Conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al conectar con el servidor.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Conexión establecida con el servidor.\n";

    // 5. Enviar mensaje al servidor
    send(clientSocket, message, strlen(message), 0);
    std::cout << "Enviando mensaje: " << message << "\n";

    // 6. Recibir respuesta del servidor
    int recvLen = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvLen > 0) {
        std::cout << "Respuesta del servidor: " << buffer << "\n";
    }

    // 7. Cerrar el socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
