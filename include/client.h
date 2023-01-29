#ifndef CLIENT_H
#define CLIENT_H
#include "basehost.h"

class client : public baseHost
{
public:
    client(sockaddr_in s, __socket_type t);
    ~client() = default;

    size_t sendMsg(QString&) override;
    size_t recvMsg(char* b, size_t s) override;

    int initHost() override;
    int acceptConnection() override;
};

#endif // CLIENT_H
