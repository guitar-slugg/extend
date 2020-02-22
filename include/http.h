#ifndef HTTP_H
#define HTTP_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "utils.h"

namespace extend
{
class SimpleHttpServer
{
public:
    SimpleHttpServer(int port)
    {
        if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(this->server_fd, SOL_SOCKET, SO_KEEPALIVE,
                       &this->opt, sizeof(this->opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        this->address.sin_family = AF_INET;
        this->address.sin_addr.s_addr = INADDR_ANY;
        this->address.sin_port = htons(port);

        if (bind(this->server_fd, (struct sockaddr *)&this->address,
                 sizeof(this->address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(this->server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    };

    void run()
    {
        this->running = true;
        while (this->running)
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            valread = read(new_socket, buffer, 1024);
            print(buffer);
            std::string resp = header + "hello";
            send(new_socket, resp.c_str(), strlen(resp.c_str()), 0);
            close(new_socket);
        }
    };

    void stop()
    {
        this->running = false;
    }

private:
    volatile bool running;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    std::string header = "HTTP/1.1 200 OK, Content-Type: text/html; charset=UTF-8, Content-Encoding: UTF-8, Accept-Ranges: bytes, Content-Length: 12, Connection: keep-alive \n\n";
};

} // namespace extend

#endif
