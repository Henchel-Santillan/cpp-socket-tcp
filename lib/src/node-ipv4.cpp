#include "node-ipv4.h"

NodeIpv4::NodeIpv4()
    : m_sockInfo(), m_socketfd(-1), m_port(DEFAULT_PORT), m_addrlen(0),
      m_buflen(DEFAULT_BUFLEN), m_state(State::CLOSED) { setSockParams(); }

NodeIpv4::NodeIpv4(const int port)
    : m_sockInfo(), m_socketfd(-1), m_port(port), m_addrlen(0),
      m_buflen(DEFAULT_BUFLEN), m_state(State::CLOSED) { setSockParams(); }

NodeIpv4::~NodeIpv4()
{
    nclose();
}

int NodeIpv4::socketFileDescriptor() const noexcept
{
    return m_socketfd;
}

void NodeIpv4::setSocketFileDescriptor(const int socketfd) noexcept
{
    m_socketfd = socketfd;
}

[[maybe_unused]] int NodeIpv4::port() const noexcept
{
    return m_port;
}

[[maybe_unused]] void NodeIpv4::setPort(const int port) noexcept
{
    m_port = port;
}

[[maybe_unused]] sockaddr_in NodeIpv4::socketInfo() const noexcept
{
    return m_sockInfo;
}

void NodeIpv4::setSocketInfo(const sockaddr_in &socketInfo) noexcept
{
    m_sockInfo = socketInfo;
}

[[maybe_unused]] State NodeIpv4::state() const noexcept
{
    return m_state;
}

void NodeIpv4::setState(const State &state) noexcept
{
    m_state = state;
}


// PROTECTED METHODS
void NodeIpv4::sendTo(const int socketfd, const std::string &message)
{
    send(socketfd, message.c_str(), message.size(), 0);
}

void NodeIpv4::setSockParams()
{
    m_addrlen = sizeof(m_sockInfo);
    std::memset(&m_sockInfo, 0, reinterpret_cast<size_t>(&m_addrlen));
    m_sockInfo.sin_family = AF_INET;
    m_sockInfo.sin_port = htons(m_port);
}

void NodeIpv4::open()
{
    #ifdef _WIN32
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw std::runtime_error("WSAStartup failed.");
    #endif

    m_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socketfd == -1)
        throw std::runtime_error("Failed to create socket.");
    m_state = State::OPEN;
}

void NodeIpv4::nclose()
{
    if (m_state != State::CLOSED)
    {
        #ifdef _WIN32
            if (::closesocket(m_socketfd) < 0)
                    throw std::runtime_error("Failed to close socket.");
                ::WSACleanup();
        #else
            if (::close(m_socketfd) < 0)
                throw std::runtime_error("Failed to nclose socket.");
        #endif
        m_state = State::CLOSED;
    }
}

bool NodeIpv4::operator==(const NodeIpv4 &other) const
{
    return m_socketfd == other.m_socketfd && m_port == other.m_port;
}

bool NodeIpv4::operator!=(const NodeIpv4 &other) const
{
    return !operator==(other);
}