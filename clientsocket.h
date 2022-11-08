#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include "basesocket.h"

class clientSocket : public baseSocket
{
public:
    clientSocket(__socket_type t, sockaddr_in a);
    ~clientSocket();

    int initSocket() override;
    void closeSocket() override;
    int acceptConnection() override;

    size_t sendMsg(QString&) override;
    size_t recvMsg() override;
};

#endif // CLIENTSOCKET_H
