#include <iostream>
#include <limits>
#include <csignal>
#include "../include/tcp-server.h"

TcpServer *server = nullptr;
void signalExit(int);
void msgOut(const std::string&);

int main()
{
    signal(SIGINT, signalExit);

    int port = DEFAULT_PORT;
    std::cout << "Port: ";
    std::cin >> port;
    std::cout << "Connection Size: ";
    int connection_t = DEFAULT_CONNSIZE;
    std::cin >> connection_t;

    try
    {
        server = new TcpServer(port, connection_t);
        server->setOnMessage(msgOut);
        server->run();
        delete server;
        server = nullptr;
    }

    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

void signalExit(int sCode)
{
    char res;
    std::cout << "Close socket connection? [Y/N]";
    std::cin >> res;
    if (toupper(res) == 'Y' && server != nullptr)
    {
        delete server;
        server = nullptr;
        exit(0);
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void msgOut(const std::string &message)
{
    std::cout << message << std::endl;
}

