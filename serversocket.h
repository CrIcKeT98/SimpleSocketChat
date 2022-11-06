#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include "basesocket.h"

class serverSocket : public baseSocket
{
public:
    serverSocket(__socket_type t, sockaddr_in a);
    ~serverSocket();

    int initSocket() override;
    void acceptConnection();
    void closeSocket() override;

private:
    int m_slaveSocket;
};

#endif // SERVERSOCKET_H
