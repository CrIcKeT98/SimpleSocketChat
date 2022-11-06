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
};

#endif // CLIENTSOCKET_H
