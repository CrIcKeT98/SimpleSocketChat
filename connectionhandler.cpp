#include "connectionhandler.h"
#include "clientsocket.h"
#include "serversocket.h"
#include <QDebug>

connectionHandler::connectionHandler(QWidget* parent)
{

}

void connectionHandler::createClient(__socket_type t, sockaddr_in a){
    m_app_type = applicationType::CLIENT;

    if(m_clientHandler == nullptr)
        m_clientHandler = std::make_unique<clientSocket>(t, a);

    if(m_clientHandler->initSocket() == 0)
        m_state = connectionStatus::ESTABLISHED;
    else
        m_state = connectionStatus::CONNECT_ERROR;
}

void connectionHandler::createServer(__socket_type t, sockaddr_in a){
    m_app_type = applicationType::SERVER;

    if(m_serverHandler == nullptr)
        m_serverHandler = std::make_unique<serverSocket>(t, a);

    int l_state = m_serverHandler->initSocket();

    if(l_state == 0)
        m_state = connectionStatus::SERVER_READY;
    else if(l_state == -1)
        m_state = connectionStatus::SOCKET_ERROR;
    else if(l_state == -2)
        m_state = connectionStatus::BIND_ERROR;
    else if(l_state == 3)
        m_state = connectionStatus::LISTEN_ERROR;
}

size_t connectionHandler::sendMessage(QString s){
    if(this->m_app_type==applicationType::SERVER)
        m_serverHandler->sendMsg(s, s.size());
    else
        m_clientHandler->sendMsg(s, s.size());

    return 0;
}

void connectionHandler::recvMessage(){
    int l_result;
    if(m_app_type == applicationType::CLIENT){
        while(true){
            l_result = m_clientHandler->recvMsg();
            if(l_result == 0)
                emit signalCloseSocket();
            else
                emit signalRecvMsg();
        }
    }
    else{
        while(true){
            m_serverHandler->recvMsg();
            emit signalRecvMsg();
        }
    }
}

char* connectionHandler::getRecvMessage(){
    if(m_app_type == applicationType::CLIENT)
        return m_clientHandler->getRecvBuff();
    else
        return m_serverHandler->getRecvBuff();
}

void connectionHandler::createWorkCycle(){
    if(m_app_type == applicationType::CLIENT)
        m_clientHandler->getSocket();
    else
        m_serverHandler->getSocket();

    m_recvThread = std::thread(&connectionHandler::recvMessage, std::ref(*this));
    m_recvThread.detach();
}

connectionStatus connectionHandler::getConnectionState(){
    return m_state;
}

void connectionHandler::closeSocket(){
    if(m_app_type == applicationType::CLIENT){
        m_clientHandler->closeSocket();
    }
    else
        m_serverHandler->closeSocket();
}
