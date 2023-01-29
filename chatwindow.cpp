#include <QTime>
#include <QMessageBox>
#include <QScrollBar>
#include "chatwindow.h"
#include "ui_chatwindow.h"

extern connectionManager g_connectionManager;

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    if(g_connectionManager.getAppType() == applicationType::CLIENT)
        this->setWindowTitle("Client");
    else
        this->setWindowTitle("Server");

    getHostIp();
    getHostPort();
    connectSignalsToSlots();

    ui->ipLine->setText(m_hostIp);
    ui->portLine->setText(m_hostPort);
    ui->socketTypeLine->setText(g_connectionManager.getSocketType() == SOCK_STREAM ? "TCP" : "UDP");

    ui->sendMsgTextEdit->setFocus();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::connectSignalsToSlots(){
    connect(&g_connectionManager, SIGNAL(signalRecvMsg()), this, SLOT(slotRecvMsg()));
    connect(&g_connectionManager, SIGNAL(signalCloseSocket()), this, SLOT(slotCloseSocket()));
    connect(&g_connectionManager, SIGNAL(signalAcceptConnection()), this, SLOT(slotAcceptConnection()));
}

void ChatWindow::getHostIp(){
    QEventLoop l_eventLoop;
    QNetworkAccessManager l_networkManager;
    QNetworkRequest l_request(QUrl("http://ipinfo.io/ip"));

    QObject::connect(&l_networkManager, SIGNAL(finished(QNetworkReply*)), &l_eventLoop, SLOT(quit()));

    std::unique_ptr<QNetworkReply> reply(l_networkManager.get(l_request));
    l_eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError)
        m_hostIp = reply->readAll();
    else
        m_hostIp = "Error";
}

void ChatWindow::getHostPort(){
    m_hostPort = QString::number(g_connectionManager.getInternalHostPort());
}

//////////////////////////////
// SLOTS
//////////////////////////////

void ChatWindow::on_sendButton_clicked()
{
    g_connectionManager.sendMsg(ui->sendMsgTextEdit->text());
    ui->sendMsgTextEdit->clear();
}

void ChatWindow::slotRecvMsg(){
    QString result_str = QTime::currentTime().toString();
    QString buff = QString(g_connectionManager.getRecvMessage());
    result_str += ": " + buff + '\n';

    ui->recvMsgTextEdit->insertPlainText(result_str);

    //Auto-scroll
    QScrollBar* l_p = ui->recvMsgTextEdit->verticalScrollBar();
    l_p->setValue(l_p->maximum());

    g_connectionManager.clearBuffs();
}

void ChatWindow::slotCloseSocket(){
    QMessageBox msg;
    QMessageBox::warning(&msg, "Warning!", "Your partner has closed the chat!");

    g_connectionManager.closeSocket();
}

void ChatWindow::slotAcceptConnection(){
    QMessageBox msg;
    QMessageBox::warning(&msg, "Notify!", "Client connected!");

    //startTransmission();
}
