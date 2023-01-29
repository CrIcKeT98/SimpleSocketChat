#include "basehost.h"

baseHost::baseHost(sockaddr_in s, __socket_type t) : m_addr_in(s), m_socket_type(t)
{
}

baseHost::~baseHost(){
    baseHost::closeHost();
}

void baseHost::closeHost(){
    shutdown(m_socket_fd, SHUT_RDWR);
    close(m_socket_fd);
}

size_t baseHost::getInternalPort() const{
    return htons(m_addr_in.sin_port);
}

__socket_type baseHost::getSocketType() const{
    return m_socket_type;
}
