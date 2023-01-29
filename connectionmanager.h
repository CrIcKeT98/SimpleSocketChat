#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <thread>
#include "basehost.h"

const size_t RECV_BUFF = 1400;

enum applicationType{
    CLIENT,
    SERVER
};

enum connectionStatus{
    SOCKET_ERROR,
    SERVER_READY,
    ESTABLISHED,
    FINALIZED,
    CONNECTING,
    BIND_ERROR,
    LISTEN_ERROR,
    CONNECT_ERROR
};

class connectionManager : public QObject
{
    Q_OBJECT
public:
    connectionManager();
    ~connectionManager() = default;

    connectionManager(const connectionManager&) = delete;
    connectionManager& operator=(const connectionManager&) = delete;

    void createClient(sockaddr_in, __socket_type);
    void createServer(sockaddr_in, __socket_type);
    void createReadThread();
    void createAcceptConnectionThread();

    void closeSocket(); // ???

    size_t sendMsg(QString);
    void recvMsg();
    void acceptConnection();
    void clearBuffs();

    QString getRecvMessage(); // ???
    connectionStatus getConnectionState();
    applicationType getAppType();
    __socket_type getSocketType();
    size_t getInternalHostPort();

private:
    std::unique_ptr<baseHost> m_hostHandler;
    applicationType m_app_type;
    connectionStatus m_connectionState;

    std::unique_ptr<char[]> m_recvBuff;
    std::unique_ptr<char[]> m_sendBuff;

    std::thread m_recvThread;
    std::thread m_acceptConnectionThread;

signals:
    void signalRecvMsg();
    void signalCloseSocket();
    void signalAcceptConnection();
};

#endif // CONNECTIONMANAGER_H
