#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "connectionmanager.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    void connectSignalsToSlots();
    void getHostIp();
    void getHostPort();

    QString m_hostIp;
    QString m_hostPort;

    Ui::ChatWindow *ui;

private slots:
    void on_sendButton_clicked();
    void slotRecvMsg();
    void slotCloseSocket();
    void slotAcceptConnection();
};

#endif // CHATWINDOW_H
