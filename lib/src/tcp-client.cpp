#include "tcp-client.h"

TcpClient::TcpClient()
    : NodeIpv4(), m_handler(nullptr) { loadtotb(); }

[[maybe_unused]] TcpClient::TcpClient(const int port)
    : NodeIpv4(port), m_handler(nullptr) { loadtotb(); }

TcpClient::TcpClient(std::string serverName, const int port)
    : NodeIpv4(port), m_serverName(std::move(serverName)), m_handler(nullptr) { loadtotb(); }

TcpClient::~TcpClient()
{
    terminate();
}

void TcpClient::connectTo()
{
    connect();
    report("Connected to server successfully.");
    read();
    write();
}

void TcpClient::terminate()
{
    this->nclose();
    if (m_thread.joinable())
        m_thread.join();
}

void TcpClient::setOnMessage(const std::function<void(const std::string &)> &function)
{
    m_handler = function;
}


// PROTECTED METHODS
void TcpClient::loadtotb()
{
    inet_pton(AF_INET, m_serverName.c_str(), &m_sockInfo.sin_addr);
}

void TcpClient::connect()
{
    this->open();
    if (::connect(m_socketfd, reinterpret_cast<sockaddr*>(&m_sockInfo), m_addrlen) == -1)
        throw std::runtime_error("Failed to connect to server.");
}

void TcpClient::write()
{
    std::string message;
    while (m_state == State::OPEN)
    {
        std::getline(std::cin, message);
        sendTo(m_socketfd, message);
    }
}

void TcpClient::read()
{
    m_thread = std::move(std::thread([ = ] {
        char buffer[m_buflen];
        long length;
        while ((length = recv(m_socketfd, buffer, m_buflen, 0)) > 0)
        {
            buffer[length] = '\0';
            report(buffer);
            std::memset(buffer, '\0', sizeof(buffer));
        }
        m_state = State::CLOSED;
    }));
}


void TcpClient::report(const std::string &message)
{
    if (m_handler != nullptr)
        m_handler(message);
}