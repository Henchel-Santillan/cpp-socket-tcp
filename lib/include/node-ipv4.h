#ifndef CPP_WINSOCK_TCP_NODE_IPV4_H
#define CPP_WINSOCK_TCP_NODE_IPV4_H

#include <cstring>
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef unsigned int socklen_t
static WSAData wsaData;

#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif  // _WIN32

inline constexpr int DEFAULT_BUFLEN = 4096;
inline constexpr int DEFAULT_PORT = 5000;

enum class State {OPEN, CLOSED};

class NodeIpv4
{
public:
    NodeIpv4();
    explicit NodeIpv4(int port);
    virtual ~NodeIpv4();

    NodeIpv4(const NodeIpv4 &other) = default;
    NodeIpv4(NodeIpv4 &&other) noexcept = default;
    NodeIpv4& operator=(const NodeIpv4 &other) = default;
    NodeIpv4& operator=(NodeIpv4 &&other) noexcept = default;

    bool operator==(const NodeIpv4 &other) const;
    bool operator!=(const NodeIpv4 &other) const;

public:
    [[nodiscard]] int socketFileDescriptor() const noexcept;
    void setSocketFileDescriptor(int socketfd) noexcept;

    [[maybe_unused]] [[nodiscard]] int port() const noexcept;
    [[maybe_unused]] void setPort(int port) noexcept;

    [[maybe_unused]] sockaddr_in socketInfo() const noexcept;
    void setSocketInfo(const sockaddr_in &socketInfo) noexcept;

    [[maybe_unused]] State state() const noexcept;
    void setState(const State &state) noexcept;

protected:
    static void sendTo(const int socketfd, const std::string &message);
    void setSockParams();
    void open();
    void nclose();

protected:
    struct sockaddr_in m_sockInfo;
    int m_socketfd, m_port, m_addrlen, m_buflen;
    State m_state;
};

#endif //CPP_WINSOCK_TCP_NODE_IPV4_H
