#ifndef HTTP_H
#define HTTP_H

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "logging.h"

namespace extend
{

class RestRequestHandler
{
    public:
        virtual std::string handleRequest(const std::string &request)=0;
};

class EchoRequestHandler : public RestRequestHandler
{
    public:
        std::string handleRequest(const std::string &request)
        {
            print("handling request: " + request);
            return "you requested: " + request;
        };
};

class SimpleRestServer
{
public:
    //defualt to json type header 
    std::string header = "HTTP/1.1 200 OK, Content-Type: text/json; charset=UTF-8, Content-Encoding: UTF-8, Accept-Ranges: bytes, Connection: close, Content-Length:";
    bool appendHeader = true;
    const static int READ_BUFFER_SIZE = 1024; 

    //default to echo handler
    RestRequestHandler * reqHandler = new EchoRequestHandler();

    SimpleRestServer(int port)
    {
        if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR,
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
        if (listen(this->server_fd, BACKLOG) < 0)
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

            valread = read(new_socket, buffer, READ_BUFFER_SIZE);

            //get return data
            std::string resp = this->reqHandler->handleRequest(this->parseReq(buffer));

            //append header
            //header can be handled in RestRequestHandler if desired 
            if(this->appendHeader)
            {
                resp = this->header + std::to_string(resp.length()) +  "\n\n" +  resp;
            }

            const char * cStr = resp.c_str();
            send(new_socket, cStr, strlen(cStr), 0);

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
    char buffer[READ_BUFFER_SIZE] = {0};
    const static int BACKLOG = 3; 

    std::string parseReq(char * req)
    { 
        std::string reqStr(req);
        size_t found = reqStr.find(" "); 
        if(found<1)
        {
            return reqStr;
        }
        int stopInd = reqStr.find(" ", found +1); 
        return reqStr.substr(0, stopInd);
    }
};

} // namespace extend

#endif
