#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include "basesocket.h"

class serverSocket : public baseSocket
{
public:
    serverSocket(__socket_type t, sockaddr_in a);
    ~serverSocket();

    int initSocket() override;
    void closeSocket() override;
    int acceptConnection() override;

    size_t sendMsg(QString&) override;
    size_t recvMsg() override;

private:
    int m_slaveSocket;
};

#endif // SERVERSOCKET_H
