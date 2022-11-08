#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include "basesocket.h"

class serverSocket : public baseSocket
{
public:
    serverSocket(__socket_type t, sockaddr_in a);
    ~serverSocket();

    size_t sendMsg(QString&) override;
    size_t recvMsg() override;

    int initSocket() override;
    void acceptConnection() override;
    void closeSocket() override;

private:
    int m_slaveSocket;
};

#endif // SERVERSOCKET_H
