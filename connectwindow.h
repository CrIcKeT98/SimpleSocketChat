#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include "connectionmanager.h"
#include "chatwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ConnectWindow; }
QT_END_NAMESPACE

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

private:
    void initWindow();

    bool validateWindow();
    bool validateIP();
    bool validatePort();
    bool validateStreamType();

    int getWindowData(sockaddr_in&);
    uint16_t getPort();

    std::unique_ptr<ChatWindow> m_chatWindow;
    Ui::ConnectWindow *ui;

private slots:
    void on_connectButton_clicked();
    void on_createServerButton_clicked();
};
#endif // CONNECTWINDOW_H
