#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include "connectionhandler.h"
#include "chatwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectWindow; }
QT_END_NAMESPACE

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConnectWindow(connectionHandler& h, QWidget *parent = nullptr);
    ~ConnectWindow();

private:
    void initWindow();
    void connectSignalsToSlots();

    bool validateWindow();
    bool validateIP();
    bool validatePort();
    bool validateStreamType();

    int getWindowData(sockaddr_in&);
    void getPort(sockaddr_in&);

    connectionHandler& m_connectionHandle;
    std::unique_ptr<ChatWindow> m_chatWindow;
    Ui::ConnectWindow *ui;

private slots:
    void on_connectButton_clicked();
    void on_createNewConnectButton_clicked();
};
#endif // CONNECTWINDOW_H
