#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "connectionhandler.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(connectionHandler&, QWidget *parent = nullptr);
    ~ChatWindow();

    void startTransmission();

private:
    connectionHandler& m_connectionHandle;
    Ui::ChatWindow *ui;

private slots:
    void on_sendButton_clicked();
    void slotRecvMsg();
    void slotCloseSocket();
    void slotAcceptConnection();
};

#endif // CHATWINDOW_H
