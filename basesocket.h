#ifndef BASESOCKET_H
#define BASESOCKET_H
#include <QString>
#include <memory>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const size_t RECV_BUFF = 1400;

class baseSocket
{
public:
    baseSocket(__socket_type t, sockaddr_in a);
    virtual ~baseSocket() = default;

    virtual int initSocket() = 0;
    virtual void closeSocket() = 0;
    virtual int acceptConnection() = 0;

    virtual size_t sendMsg(QString&) = 0;
    virtual size_t recvMsg() = 0;

    void clearBuff();

    int getSocket() const;
    char* getRecvBuff() const;
    __socket_type getSocketType() const;
    int getSocketPort() const;

protected:
    int m_socket;
    __socket_type m_socket_type;
    sockaddr_in m_addr_in;
    std::unique_ptr<char[]> m_recvBuff;
    size_t m_countRecvBytes;
};

#endif // BASESOCKET_H
