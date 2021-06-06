#ifndef CPP_WINSOCK_TCP_TCP_CLIENT_H
#define CPP_WINSOCK_TCP_TCP_CLIENT_H

#include "node-ipv4.h"
#include <iostream>
#include <functional>
#include <thread>


class TcpClient : protected NodeIpv4
{
public:
    TcpClient();
    [[maybe_unused]] explicit TcpClient(int port);
    TcpClient(std::string serverName, int port);
    ~TcpClient() override;

    TcpClient(const TcpClient &other) = delete;
    TcpClient(TcpClient &&other) noexcept = delete;
    TcpClient& operator=(const TcpClient &other) = delete;
    TcpClient& operator=(TcpClient &&other) noexcept = delete;

public:
    void connectTo();
    void terminate();
    void setOnMessage(const std::function<void(const std::string&)> &function);

protected:
    void loadtotb();
    void connect();
    void write();
    void read();
    void report(const std::string &message);

protected:
    std::thread m_thread;
    std::function<void(const std::string &message)> m_handler;
    std::string m_serverName;
};

#endif //CPP_WINSOCK_TCP_TCP_CLIENT_H
