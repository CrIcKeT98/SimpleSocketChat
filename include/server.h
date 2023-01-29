#ifndef SERVER_H
#define SERVER_H
#include "basehost.h"

class server : public baseHost
{
public:
    server(sockaddr_in a, __socket_type t);
    ~server() = default;

    int initHost() override;
    size_t sendMsg(QString&) override;
    size_t recvMsg(char* b, size_t s) override;
    int acceptConnection() override;

private:
    int m_slaveSocket;
    sockaddr_in m_slaveAddr;
    socklen_t m_len;
};

#endif // SERVER_H
