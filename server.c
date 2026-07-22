#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

int main()
{
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    socklen_t clientSize = sizeof(clientAddress);

    // Let child processes exit without becoming zombies
    signal(SIGCHLD, SIG_IGN);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0)
    {
        printf("Socket creation failed.\n");
        return 0;
    }
    printf("Server socket created.\n");

    // Server information
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Server bind failed.\n");
        close(serverSocket);
        return 0;
    }
    printf("Server bind successful.\n");

    // Listen
    listen(serverSocket, 5);
    printf("Server is waiting for clients...\n");

    while(1)
    {
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientSize);
        if(clientSocket < 0)
        {
            printf("Connection failed.\n");
            continue;
        }
        printf("\nClient connected successfully.\n");

        pid_t pid = fork();

        if(pid < 0)
        {
            // fork failed, just close this client and keep going
            printf("Fork failed.\n");
            close(clientSocket);
            continue;
        }

        if(pid == 0)
        {
            // ---- Child process: handles this one client ----
            close(serverSocket); // child doesn't need the listening socket

            // Authentication
            char username[20];
            char password[20];
            int n;

            n = recv(clientSocket, username, sizeof(username) - 1, 0);
            if(n <= 0) { close(clientSocket); exit(0); }
            username[n] = '\0';

            n = recv(clientSocket, password, sizeof(password) - 1, 0);
            if(n <= 0) { close(clientSocket); exit(0); }
            password[n] = '\0';

            if(strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0)
            {
                send(clientSocket, "Authentication Successful",
                     strlen("Authentication Successful") + 1, 0);
            }
            else
            {
                send(clientSocket, "Authentication Failed",
                     strlen("Authentication Failed") + 1, 0);
                close(clientSocket);
                exit(0);
            }

            // Receive message
            char message[100];
            int bytes = recv(clientSocket, message, sizeof(message) - 1, 0);
            if(bytes <= 0)
            {
                printf("Receive Error\n");
                close(clientSocket);
                exit(0);
            }
            message[bytes] = '\0';

            if(strlen(message) == 0)
            {
                send(clientSocket, "Invalid Message", strlen("Invalid Message") + 1, 0);
            }
            else
            {
                printf("Message from client (pid %d): %s\n", getpid(), message);
                send(clientSocket, "Message received successfully.",
                     strlen("Message received successfully.") + 1, 0);
            }

            close(clientSocket);
            printf("Client disconnected (pid %d).\n", getpid());
            exit(0);
        }
        else
        {
            // ---- Parent process: goes back to accepting new clients ----
            close(clientSocket); // parent doesn't need this client's socket
        }
    }

    close(serverSocket);
    return 0;
}
