#include <QMessageBox>
#include "serversocket.h"

serverSocket::serverSocket(__socket_type t, sockaddr_in a) : baseSocket(t, a)
{

}

int serverSocket::initSocket(){
    m_socket = socket(AF_INET, m_socket_type, 0);

    if(m_socket < 0)
        return -1;

    m_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(m_socket, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0)
        return -2;

    if(listen(m_socket, SOMAXCONN) < 0)
        return -3;
}

void serverSocket::acceptConnection(){
    m_slaveSocket = accept(m_socket, 0,0);
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
