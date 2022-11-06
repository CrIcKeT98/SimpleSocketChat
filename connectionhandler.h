#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <thread>
#include <basesocket.h>

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

class connectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit connectionHandler(QWidget *parent = nullptr);
    ~connectionHandler() = default;

    size_t sendMessage(QString);
    void recvMessage();
    void closeSocket();

    void createClient(__socket_type, sockaddr_in);
    void createServer(__socket_type, sockaddr_in);
    void createWorkCycle();

    connectionStatus getConnectionState();
    char* getRecvMessage();

private:
    std::unique_ptr<baseSocket> m_serverHandler;
    std::unique_ptr<baseSocket> m_clientHandler;
    applicationType m_app_type;
    connectionStatus m_state;

    std::thread m_recvThread;

signals:
    void signalRecvMsg();
    void signalCloseSocket();
};

#endif // CONNECTIONHANDLER_H
