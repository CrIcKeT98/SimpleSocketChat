#include "connectwindow.h"
#include "ui_connectwindow.h"
#include <QRegularExpressionValidator>
#include <QMessageBox>

ConnectWindow::ConnectWindow(connectionHandler& h, QWidget *parent)
    : m_connectionHandle(h)
    ,QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);

    initWindow();
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::initWindow(){
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QString portRange = "(^(([0-9]{1,4})|([1-5][0-9]{4})|(6[0-4][0-9]{3})|(65[0-4][0-9]{2})|(655[0-2][0-9])|(6553[0-5]))$)";

    QRegularExpression ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");

    QRegularExpression portRegex(portRange);

    QRegularExpressionValidator *ipValidator = new QRegularExpressionValidator(ipRegex, this);
    QRegularExpressionValidator *portValidator = new QRegularExpressionValidator(portRegex, this);

    ui->ipLine->setValidator(ipValidator);
    ui->portLine->setValidator(portValidator);
}

bool ConnectWindow::validateWindow(){
    return validateIP() & validatePort() & validateStreamType();
}

bool ConnectWindow::validateIP(){
    //TODO: check IP correctness
    if(ui->ipLine->text().isEmpty()){
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "IP is not entered or uncorrected!");
        return false;
    }

    return true;
}

bool ConnectWindow::validatePort(){
    if(ui->portLine->text().isEmpty()){
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "Port is not entered!");
        return false;
    }

    return true;
}

bool ConnectWindow::validateStreamType(){
    if(!ui->tcpButton->isChecked() && !ui->udpButton->isChecked()){
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "Protocol is not selected!");
        return false;
    }

    return true;
}

void ConnectWindow::getPort(sockaddr_in& sock_buff){
    sock_buff.sin_family = AF_INET;

    if(ui->portLine->text().isEmpty())
        sock_buff.sin_port = 0;
    else
        sock_buff.sin_port = htons(ui->portLine->text().toInt());
}

int ConnectWindow::getWindowData(sockaddr_in& sock_buff){
    QString l_buff = ui->ipLine->text();

    if(inet_pton(AF_INET, l_buff.toStdString().c_str(), &(sock_buff.sin_addr)) == 0){
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "Entered IP is not corrected!");
        return 0;
    }

    getPort(sock_buff);

    return 1;
}

void ConnectWindow::connectSignalsToSlots(){
    connect(&this->m_connectionHandle, SIGNAL(signalRecvMsg()), m_chatWindow.get(), SLOT(slotRecvMsg()));
    connect(&this->m_connectionHandle, SIGNAL(signalCloseSocket()), m_chatWindow.get(), SLOT(slotCloseSocket()));
}

void ConnectWindow::on_connectButton_clicked()
{
    if(!validateWindow())
        return;

    sockaddr_in sock_buff;
    memset(&sock_buff, 0, sizeof (sock_buff));

    if(!getWindowData(sock_buff))
        return;

    m_connectionHandle.createClient(ui->tcpButton->isChecked() ? SOCK_STREAM : SOCK_DGRAM, sock_buff);

    if(m_connectionHandle.getConnectionState() != connectionStatus::ESTABLISHED){
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "Failed to connect to the server!");
        return;
    }

    m_chatWindow = std::make_unique<ChatWindow>(m_connectionHandle);

    connectSignalsToSlots();
    m_chatWindow->show();
    m_chatWindow->startTransmission();

    this->close();
}


void ConnectWindow::on_createNewConnectButton_clicked()
{    
    if(!validateStreamType())
        return;

    sockaddr_in sock_buff;
    memset(&sock_buff, 0, sizeof (sock_buff));

    getPort(sock_buff);

    m_connectionHandle.createServer(ui->tcpButton->isChecked() ? SOCK_STREAM : SOCK_DGRAM, sock_buff);

    if(m_connectionHandle.getConnectionState() != connectionStatus::SERVER_READY){
        QMessageBox msg;
        if(m_connectionHandle.getConnectionState() == connectionStatus::SOCKET_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to create socket!");
            return;
        }
        else if(m_connectionHandle.getConnectionState() == connectionStatus::BIND_ERROR){
            QMessageBox::warning(&msg, "Error!", "Port is busy!");
            return;
        }
        else if(m_connectionHandle.getConnectionState() == connectionStatus::LISTEN_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to call listen()!");
            return;
        }
    }

    m_chatWindow = std::make_unique<ChatWindow>(m_connectionHandle);

    connectSignalsToSlots();

    m_connectionHandle.acceptConnection();
    m_chatWindow->show();
    m_chatWindow->startTransmission();

    this->close();
}

