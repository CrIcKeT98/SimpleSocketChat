#include <QTime>
#include <QMessageBox>
#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(connectionHandler& h, QWidget *parent) :
    m_connectionHandle(h),
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->sendMsgTextEdit->setFocus();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::startTransmission(){
    m_connectionHandle.createWorkCycle();
}

void ChatWindow::on_sendButton_clicked()
{
    m_connectionHandle.sendMessage(ui->sendMsgTextEdit->text());
    ui->sendMsgTextEdit->clear();
}

void ChatWindow::slotRecvMsg(){
    QString result_str = QTime::currentTime().toString();
    QString buff = QString(m_connectionHandle.getRecvMessage());
    result_str += ": " + buff + '\n';

    ui->recvMsgTextEdit->insertPlainText(result_str);
    m_connectionHandle.clearBuff();
}

void ChatWindow::slotCloseSocket(){
    QMessageBox msg;
    QMessageBox::warning(&msg, "Warning!", "Your interlocutor has closed the chat!");

    m_connectionHandle.closeSocket();
}
