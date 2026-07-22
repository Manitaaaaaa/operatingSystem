#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;
    char message[100];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0)
    {
        printf("Socket creation failed.\n");
        return 0;
    }
    printf("Client socket created.\n");

    // Server information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    if(connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Connection failed.\n");
        close(clientSocket);
        return 0;
    }
    printf("Connected to server.\n");

    // Authentication
    char username[20];
    char password[20];
    char buffer[100];

    printf("Username: ");
    scanf("%19s", username);
    printf("Password: ");
    scanf("%19s", password);

    send(clientSocket, username, strlen(username), 0);
    send(clientSocket, password, strlen(password), 0);

    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if(bytes <= 0)
    {
        printf("Receive Error\n");
        close(clientSocket);
        return 0;
    }
    buffer[bytes] = '\0';
    printf("%s\n", buffer);

    if(strcmp(buffer, "Authentication Failed") == 0)
    {
        close(clientSocket);
        return 0;
    }

    // Send message (width limit added so it can't overflow the buffer)
    printf("Enter message: ");
    scanf(" %99[^\n]", message);
    send(clientSocket, message, strlen(message), 0);

    // Receive reply
    char reply[100];
    bytes = recv(clientSocket, reply, sizeof(reply) - 1, 0);
    if(bytes > 0)
    {
        reply[bytes] = '\0';
        printf("Server: %s\n", reply);
    }

    close(clientSocket);
    return 0;
}
