#include "tcp-server.h"

TcpServer::TcpServer()
    : NodeIpv4(), m_handler(nullptr), m_connectionSize(DEFAULT_CONNSIZE), m_nodeSize(DEFAULT_NODESIZE)
{
    setInAddrSysCall();
}

[[maybe_unused]] TcpServer::TcpServer(const int port)
    : NodeIpv4(port), m_handler(nullptr), m_connectionSize(DEFAULT_CONNSIZE), m_nodeSize(DEFAULT_NODESIZE)
{
    setInAddrSysCall();
}

TcpServer::TcpServer(const int port, const int connectionSize)
    : NodeIpv4(port), m_handler(nullptr), m_connectionSize(connectionSize), m_nodeSize(DEFAULT_NODESIZE)
{
    setInAddrSysCall();
}

TcpServer::~TcpServer()
{
    shutdown();
}

void TcpServer::run()
{
    create();
    sbind();
    slisten();
    handleRequest();
}

void TcpServer::shutdown()
{
    sendToClients("Server closed.");
    closeClients();
    nclose();
    joinThreads();
    m_threads.clear();
}

void TcpServer::setOnMessage(const std::function<void(const std::string &)> &function)
{
    m_handler = function;
}


// PROTECTED METHODS
void TcpServer::setInAddrSysCall()
{
    m_sockInfo.sin_addr.s_addr = htonl(INADDR_ANY);
}

void TcpServer::sendToClients(const std::string &message)
{
    for (auto &client : m_clients)
        sendTo(client->socketFileDescriptor(), message);
}

void TcpServer::sendToAll(const std::string &message, int socket)
{
    for (auto &client : m_clients)
    {
        if (client->socketFileDescriptor() != socket)
            sendTo(client->socketFileDescriptor(), message);
    }
}

void TcpServer::create()
{
    constexpr int OPTION = 1;
    open();

    if (setsockopt(m_socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &OPTION, sizeof(OPTION)))
        throw std::runtime_error("Could not set socket option.");
}

void TcpServer::sbind()
{
    if (bind(m_socketfd, reinterpret_cast<sockaddr*>(&m_sockInfo), m_addrlen) == -1)
        throw std::runtime_error("Failed to bind socket.");
}

void TcpServer::slisten()
{
    if (listen(m_socketfd, m_connectionSize) == -1)
        throw std::runtime_error("Failed to open socket for listening.");
}

void TcpServer::handleRequest()
{
    report("Server running ...");
    report(std::string(inet_ntoa(m_sockInfo.sin_addr)) + ":" + std::to_string(ntohs(m_sockInfo.sin_port)));

    while (true)
    {
        struct sockaddr_in tempAddress {};
        socklen_t tempLength = sizeof(tempAddress);
        int tempSocket = accept(m_socketfd, reinterpret_cast<sockaddr*>(&tempAddress), &tempLength);

        if (m_nodeSize + 1 > m_connectionSize)
        {
            sendTo(tempSocket, "Connection capacity exceeded: server is full");
            close(tempSocket);
            break;
        }

        m_nodeSize++;

        auto *client = new NodeIpv4();
        client->setSocketInfo(tempAddress);
        client->setSocketFileDescriptor(tempSocket);
        client->setState(State::OPEN);

        std::string clientIp(inet_ntoa(tempAddress.sin_addr));
        std::string clientPort(std::to_string(ntohs(tempAddress.sin_port)));

        std::string reportMsg;
        reportMsg.append(clientIp + ":");

        m_threads.emplace_back(std::thread([ = ] {
            char buffer[m_buflen];
            long length;
            while ((length = recv(client->socketFileDescriptor(), buffer, m_buflen, 0)) > 0)
            {
                buffer[length] = '\0';
                std::string message(clientIp);
                message.append(":" + clientPort + ">>>" + buffer);
                sendToAll(message, client->socketFileDescriptor());
                std::memset(buffer, '\0', sizeof(buffer));
            }

            m_clients.erase(std::find(m_clients.begin(), m_clients.end(), client));
            report(reportMsg + clientPort + " disconnected.");
        }));

        report(reportMsg + clientPort + " connected.");
        m_clients.push_back(client);
    }
}

void TcpServer::report(const std::string &message)
{
    if (m_handler != nullptr)
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_handler(message);
    }
}

void TcpServer::joinThreads()
{
    for (auto &thread : m_threads)
    {
        if (thread.joinable())
            thread.join();
    }
}

void TcpServer::closeClients()
{
    for (auto &client : m_clients)
    {
        delete client;
        client = nullptr;
    }
    m_clients.clear();
}