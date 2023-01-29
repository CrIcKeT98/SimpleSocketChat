#include <QMessageBox>
#include "server.h"

server::server(sockaddr_in s, __socket_type t ) : baseHost(s, t)
{
}

//TODO: check last arg in socket()
int server::initHost(){
    m_socket_fd = socket(AF_INET, m_socket_type, 0);

    if(m_socket_fd < 0)
        return -1;

    int optval = 1;
    setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if(bind(m_socket_fd, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0)
            return -2;

    if(m_socket_type == SOCK_STREAM){
        if(listen(m_socket_fd, SOMAXCONN) < 0)
            return -3;
    }

    //Get port from system if port is 0
    if(m_addr_in.sin_port == 0){
        sockaddr_in l_addr_in;
        socklen_t l_sockLen;

        if(getsockname(m_socket_fd, reinterpret_cast<sockaddr*>(&l_addr_in), &l_sockLen) == 0)
            m_addr_in.sin_port = l_addr_in.sin_port;
    }

    return 0;
}

size_t server::sendMsg(QString& s){
    if(m_socket_type == SOCK_STREAM)
        return send(m_slaveSocket, s.toStdString().c_str(), s.size(), 0);
    else
        return sendto(m_socket_fd, s.toStdString().c_str(), s.size(), 0, reinterpret_cast<sockaddr*>(&m_slaveAddr), sizeof(m_slaveAddr));
}

size_t server::recvMsg(char* b, size_t s){
    if(m_socket_type == SOCK_STREAM)
        return recv(m_slaveSocket, b, s, 0);
    else
        return recvfrom(m_socket_fd, b, s, 0, reinterpret_cast<sockaddr*>(&m_slaveAddr), &m_len);
}

int server::acceptConnection(){
    return m_slaveSocket = accept(m_socket_fd, 0,0);
}
