#ifndef CPP_WINSOCK_TCP_TCP_SERVER_H
#define CPP_WINSOCK_TCP_TCP_SERVER_H

#include "node-ipv4.h"

#include <vector>
#include <functional>
#include <algorithm>
#include <thread>
#include <mutex>

inline constexpr int DEFAULT_CONNSIZE = 128;
inline constexpr int DEFAULT_NODESIZE = 5;

class TcpServer : protected NodeIpv4
{
public:
    TcpServer();
    [[maybe_unused]] explicit TcpServer(int port);
    TcpServer(int port, int connectionSize);
    ~TcpServer() override;

    TcpServer(const TcpServer &other) = delete;
    TcpServer(TcpServer &&other) noexcept = delete;
    TcpServer& operator=(const TcpServer &other) = delete;
    TcpServer& operator=(TcpServer &&other) noexcept = delete;

public:
    void run();
    void shutdown();
    void setOnMessage(const std::function<void(const std::string&)> &function);

protected:
    void sendToClients(const std::string &message);
    void sendToAll(const std::string &message, int socket);
    void create();
    void sbind();
    void slisten();
    void handleRequest();
    void report(const std::string &message);
    void joinThreads();
    void closeClients();
    void setInAddrSysCall();

protected:
    std::mutex m_mutex;
    std::vector<std::thread> m_threads;
    std::vector<NodeIpv4*> m_clients;
    std::function<void(const std::string &message)> m_handler;
    int m_connectionSize, m_nodeSize;
};

#endif //CPP_WINSOCK_TCP_TCP_SERVER_H
