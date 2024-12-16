#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // Librería de sockets para Windows

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[1024] = { 0 };

    // 1. Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al inicializar Winsock.\n";
        return 1;
    }

    // 2. Crear socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket.\n";
        WSACleanup();
        return 1;
    }

    // 3. Configurar la opción SO_REUSEADDR
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        std::cerr << "Error en setsockopt.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 4. Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9090); // Puerto 9090

    // 5. Enlazar el socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al enlazar el socket.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 6. Escuchar conexiones entrantes
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Error al escuchar en el puerto.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Esperando conexiones en el puerto 9090...\n";

    // 7. Aceptar una conexión del cliente
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al aceptar la conexión.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 8. Recibir mensaje del cliente
    int recvLen = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (recvLen > 0) {
        std::cout << "Mensaje recibido del cliente: " << buffer << "\n";
    }

    // 9. Enviar respuesta al cliente
    const char* response = "¡Hola desde el servidor!";
    send(clientSocket, response, strlen(response), 0);
    std::cout << "Respuesta enviada al cliente.\n";

    // 10. Cerrar sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
