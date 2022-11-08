#include "clientsocket.h"

clientSocket::clientSocket(__socket_type t, sockaddr_in a) : baseSocket(t, a)
{

}

clientSocket::~clientSocket(){
    clientSocket::closeSocket();
}

int clientSocket::initSocket(){
    if(connect(m_socket, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0)
        return -1;

    return 0;
}

void clientSocket::closeSocket(){
    shutdown(m_socket, SHUT_RDWR);
    close(m_socket);
}

//Dummy function
int clientSocket::acceptConnection(){
    return -1;
}

size_t clientSocket::sendMsg(QString& s){
    return send(m_socket, s.toStdString().c_str(), s.size(), 0);
}

size_t clientSocket::recvMsg(){
    return m_countRecvBytes = recv(m_socket, m_recvBuff.get(), RECV_BUFF, 0);
}

