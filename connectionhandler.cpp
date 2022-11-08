#include "connectionhandler.h"
#include "clientsocket.h"
#include "serversocket.h"

connectionHandler::connectionHandler(QWidget* parent)
{

}

void connectionHandler::createClient(__socket_type t, sockaddr_in a){
    m_app_type = applicationType::CLIENT;

    if(m_socketHandler == nullptr)
        m_socketHandler = std::make_unique<clientSocket>(t, a);

    if(m_socketHandler->initSocket() == 0)
        m_state = connectionStatus::ESTABLISHED;
    else
        m_state = connectionStatus::CONNECT_ERROR;
}

void connectionHandler::createServer(__socket_type t, sockaddr_in a){
    m_app_type = applicationType::SERVER;

    if(m_socketHandler == nullptr)
        m_socketHandler = std::make_unique<serverSocket>(t, a);

    int l_state = m_socketHandler->initSocket();

    if(l_state == 0)
        m_state = connectionStatus::SERVER_READY;
    else if(l_state == -1)
        m_state = connectionStatus::SOCKET_ERROR;
    else if(l_state == -2)
        m_state = connectionStatus::BIND_ERROR;
    else if(l_state == 3)
        m_state = connectionStatus::LISTEN_ERROR;
}

void connectionHandler::acceptConnection(){
    m_socketHandler->acceptConnection();
}

size_t connectionHandler::sendMessage(QString s){
    m_socketHandler->sendMsg(s);
    //TODO: return correct value
    return 0;
}

void connectionHandler::recvMessage(){
    while(true){
        if(m_socketHandler->recvMsg() == 0){
            emit signalCloseSocket();
            return;
        }
        else
            emit signalRecvMsg();
    }
}

char* connectionHandler::getRecvMessage(){
    return m_socketHandler->getRecvBuff();
}

void connectionHandler::createWorkCycle(){
    m_recvThread = std::thread(&connectionHandler::recvMessage, std::ref(*this));
    m_recvThread.detach();
}

void connectionHandler::clearBuff(){
    m_socketHandler->clearBuff();
}

connectionStatus connectionHandler::getConnectionState(){
    return m_state;
}

void connectionHandler::closeSocket(){
    m_socketHandler->closeSocket();
}
