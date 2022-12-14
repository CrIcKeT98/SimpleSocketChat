#include <QMessageBox>
#include "serversocket.h"

serverSocket::serverSocket(__socket_type t, sockaddr_in a) : baseSocket(t, a)
{

}

serverSocket::~serverSocket(){
    serverSocket::closeSocket();
}

int serverSocket::initSocket(){
    m_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0)
        return -2;

    sockaddr_in l_addr_in;
    socklen_t l_sockLen;
    if(getsockname(m_socket, reinterpret_cast<sockaddr*>(&l_addr_in), &l_sockLen) == 0)
        m_addr_in.sin_port = htons(l_addr_in.sin_port);

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

size_t serverSocket::sendMsg(QString& s){
    return send(m_slaveSocket, s.toStdString().c_str(), s.size(), 0);
}

size_t serverSocket::recvMsg(){
    return m_countRecvBytes = recv(m_slaveSocket, m_recvBuff.get(), RECV_BUFF, 0);
}
