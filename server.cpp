#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;

int main()
{
    // create a server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socker creation failed" << std::endl;
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind to a port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed" << std::endl;
        return -1;
    }

    // listen for incoming connections
    listen(server_fd, 10);

    while (true)
    {
        // accept connections
        int addrlen = sizeof(address);
        int listeningSocket_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        // read the http request
        char buffer[1024] = {0};
        read(listeningSocket_fd, buffer, 1024);
        std::cout << buffer << std::endl;

        // send back an http response
        // Simple HTTP response
        char *httpResponse = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, World!";
        write(listeningSocket_fd, httpResponse, strlen(httpResponse));

        // close the socket created for talking to the client
        close(listeningSocket_fd);
    }
    return 0;
}