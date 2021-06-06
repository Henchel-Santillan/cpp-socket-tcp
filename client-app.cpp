#include <iostream>
#include <limits>
#include <csignal>
#include "../include/tcp-client.h"

TcpClient *client = nullptr;
void signalExit(int);
void msgOut(const std::string&);

int main()
{
    signal(SIGINT, signalExit);

    std::string server = "localhost";
    std::cout << "Server: ";
    std::getline(std::cin, server);
    std::cout << "Port: ";
    int port = DEFAULT_PORT;
    std::cin >> port;

    try
    {
        client = new TcpClient(server, port);
        client->setOnMessage(msgOut);
        client->connectTo();
        delete client;
        client = nullptr;
    }

    catch (const std::exception &e)
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
    if (toupper(res) == 'Y' && client != nullptr)
    {
        delete client;
        client = nullptr;
        exit(0);
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void msgOut(const std::string &message)
{
    std::cout << message << std::endl;
}

