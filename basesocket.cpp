#include "basesocket.h"

baseSocket::baseSocket(__socket_type t, sockaddr_in a) : m_socket_type(t), m_addr_in(a), m_recvBuff(std::make_unique<char[]>(RECV_BUFF))
{
    m_socket = socket(AF_INET, m_socket_type, 0);

    /*if(m_socket < 0)
        m_socket_state = baseStatus::ERROR;
    else
        m_socket_state = baseStatus::OK;*/
}

size_t baseSocket::sendMsg(QString& s, int size){
    return send(m_socket, s.toStdString().c_str(), size, 0);
}

size_t baseSocket::recvMsg(){
    return m_countRecvBytes = recv(m_socket, m_recvBuff.get(), RECV_BUFF, 0);
}

int baseSocket::getSocket() const{
    return m_socket;
}

char* baseSocket::getRecvBuff() const{
    return m_recvBuff.get();
}
