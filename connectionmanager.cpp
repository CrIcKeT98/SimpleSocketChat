#include <QEventLoop>
#include "connectionmanager.h"
#include "client.h"
#include "server.h"

connectionManager::connectionManager() : m_recvBuff(std::make_unique<char[]>(RECV_BUFF)),
    m_sendBuff(std::make_unique<char[]>(RECV_BUFF))
{
    clearBuffs();
}

void connectionManager::createClient(sockaddr_in s, __socket_type t){
    m_app_type = applicationType::CLIENT;

    if(m_hostHandler == nullptr)
        m_hostHandler = std::make_unique<client>(s, t);

    int l_state = m_hostHandler->initHost();
    if(l_state == 0){
        m_connectionState = connectionStatus::ESTABLISHED;
        createReadThread();
    }
    else if(l_state == -1)
        m_connectionState = connectionStatus::SOCKET_ERROR;
    else if(l_state == -2)
        m_connectionState = connectionStatus::CONNECT_ERROR;
}

void connectionManager::createServer(sockaddr_in s, __socket_type t){
    m_app_type = applicationType::SERVER;

    if(m_hostHandler == nullptr)
        m_hostHandler = std::make_unique<server>(s, t);

    int l_state = m_hostHandler->initHost();
    if(l_state == 0){
        m_connectionState = connectionStatus::SERVER_READY;
        if(m_hostHandler->getSocketType() == SOCK_STREAM){
            createAcceptConnectionThread(); //TCP
        }
        else{
            createReadThread(); //UDP
        }
    }
    else if(l_state == -1)
        m_connectionState = connectionStatus::SOCKET_ERROR;
    else if(l_state == -2)
        m_connectionState = connectionStatus::BIND_ERROR;
    else if(l_state == 3)
        m_connectionState = connectionStatus::LISTEN_ERROR;
}

void connectionManager::createReadThread(){
    m_recvThread = std::thread(&connectionManager::recvMsg, std::ref(*this));
    m_recvThread.detach();
}

void connectionManager::createAcceptConnectionThread(){
    m_acceptConnectionThread = std::thread(&connectionManager::acceptConnection, std::ref(*this));
    m_acceptConnectionThread.detach();
}

void connectionManager::closeSocket(){
    m_hostHandler->~baseHost();
}

size_t connectionManager::sendMsg(QString s){
    return m_hostHandler->sendMsg(s);
}

void connectionManager::recvMsg(){
    while(true){
        if(m_hostHandler->recvMsg(m_recvBuff.get(), RECV_BUFF) == 0){
            emit signalCloseSocket();
            return;
        }
        else
            emit signalRecvMsg();
    }
}

void connectionManager::acceptConnection(){
    if(m_hostHandler->acceptConnection() > 0){
        emit signalAcceptConnection();
        createReadThread();
        return;
    }
}

void connectionManager::clearBuffs(){
    memset(m_recvBuff.get(), 0, RECV_BUFF);
    memset(m_sendBuff.get(), 0, RECV_BUFF);
}

applicationType connectionManager::getAppType(){
    return m_app_type;
}

__socket_type connectionManager::getSocketType(){
    return this->m_hostHandler->getSocketType();
}

QString connectionManager::getRecvMessage(){
    return QString(m_recvBuff.get());
}

connectionStatus connectionManager::getConnectionState(){
    return m_connectionState;
}

size_t connectionManager::getInternalHostPort(){
    return m_hostHandler->getInternalPort();
}
