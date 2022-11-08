#include "basesocket.h"
#include <QDebug>

baseSocket::baseSocket(__socket_type t, sockaddr_in a) : m_socket_type(t), m_addr_in(a), m_recvBuff(std::make_unique<char[]>(RECV_BUFF))
{
    m_socket = socket(AF_INET, m_socket_type, 0);
}

void baseSocket::clearBuff(){
    memset(m_recvBuff.get(), 0, RECV_BUFF);
}


//Getters


int baseSocket::getSocket() const{
    return m_socket;
}

char* baseSocket::getRecvBuff() const{
    return m_recvBuff.get();
}

__socket_type baseSocket::getSocketType() const{
    return m_socket_type;
}

int baseSocket::getSocketPort() const{
    return m_addr_in.sin_port;
}
