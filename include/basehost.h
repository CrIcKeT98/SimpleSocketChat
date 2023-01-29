#ifndef BASEHOST_H
#define BASEHOST_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <memory>
#include <QString>

class baseHost
{
public:
    baseHost(sockaddr_in s, __socket_type t);
    virtual ~baseHost();

    virtual int initHost() = 0;
    virtual int acceptConnection() = 0;
    virtual void closeHost();

    virtual size_t sendMsg(QString&) = 0;
    virtual size_t recvMsg(char* b, size_t s) = 0;

    size_t getInternalPort() const;
    __socket_type getSocketType() const;

protected:
    int m_socket_fd;
    sockaddr_in m_addr_in;
    __socket_type m_socket_type;
};

#endif // BASEHOST_H
