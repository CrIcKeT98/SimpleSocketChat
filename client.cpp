#include "client.h"

client::client(sockaddr_in s, __socket_type t) : baseHost(s, t)
{
}

int client::initHost(){
    m_socket_fd = socket(AF_INET, m_socket_type, 0);

    if(m_socket_fd < 0)
        return -1;

    if(m_socket_type == SOCK_STREAM){
        if(connect(m_socket_fd, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in)) < 0){
            return -2;
        }
    }

    return 0;
}

size_t client::sendMsg(QString& s){
    if(m_socket_type == SOCK_STREAM)
        return send(m_socket_fd, s.toStdString().c_str(), s.size(), 0);
    else
        return sendto(m_socket_fd, s.toStdString().c_str(), s.size(), 0, reinterpret_cast<sockaddr*>(&m_addr_in), sizeof(m_addr_in));
}

size_t client::recvMsg(char* b, size_t s){
    socklen_t l_len;

    if(m_socket_type == SOCK_STREAM)
        return recv(m_socket_fd, b, s, 0);
    else
        return recvfrom(m_socket_fd, b, s, 0, reinterpret_cast<sockaddr*>(&m_addr_in), &l_len);
}


//Dummy function
int client::acceptConnection(){
    return -1;
}
