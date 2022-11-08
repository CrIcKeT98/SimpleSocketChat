#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

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

    void createClient(__socket_type, sockaddr_in);
    void createServer(__socket_type, sockaddr_in);
    void createWorkCycle();
    void createAcceptCycle();

    void closeSocket();
    void acceptConnection();
    void clearBuff();

    size_t sendMessage(QString);
    void recvMessage();

    char* getRecvMessage();
    connectionStatus getConnectionState();
    applicationType getAppType();
    __socket_type getSocketType();
    QString getHostIP();
    QString getHostPort();

private:
    std::unique_ptr<baseSocket> m_socketHandler;
    applicationType m_app_type;
    connectionStatus m_state;

    std::thread m_recvThread;
    std::thread m_acceptConnection;

signals:
    void signalRecvMsg();
    void signalCloseSocket();
    void signalAcceptConnection();
};

#endif // CONNECTIONHANDLER_H
