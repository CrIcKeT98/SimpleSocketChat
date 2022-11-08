#include <QMessageBox>
#include "serversocket.h"

serverSocket::serverSocket(__socket_type t, sockaddr_in a) : baseSocket(t, a)
{

}

size_t serverSocket::sendMsg(QString& s){
    return send(m_slaveSocket, s.toStdString().c_str(), s.size(), 0);
}

size_t serverSocket::recvMsg(){
    return m_countRecvBytes = recv(m_slaveSocket, m_recvBuff.get(), RECV_BUFF, 0);
}

int serverSocket::initSocket(){
    m_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0)
        return -2;

    if(listen(m_socket, SOMAXCONN) < 0)
        return -3;

    return 0;
}

int serverSocket::acceptConnection(){
    return m_slaveSocket = accept(m_socket, 0,0);
}

void serverSocket::closeSocket(){
    shutdown(m_socket, SHUT_RDWR);
    shutdown(m_slaveSocket, SHUT_RDWR);

    close(m_socket);
    close(m_slaveSocket);
}

serverSocket::~serverSocket(){
    serverSocket::closeSocket();
}
