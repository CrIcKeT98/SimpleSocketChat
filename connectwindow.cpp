#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "connectwindow.h"
#include "ui_connectwindow.h"

extern connectionManager g_connectionManager;

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ConnectWindow)
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

int ConnectWindow::getWindowData(sockaddr_in& sock_buff){
    sock_buff.sin_port = getPort();

    if(inet_pton(AF_INET,
                 ui->ipLine->text().toStdString().c_str(),
                 &(sock_buff.sin_addr.s_addr)) == 0)
    {
        QMessageBox msg;
        QMessageBox::warning(&msg, "Error!", "Entered IP is not corrected!");
        return 0;
    }

    return 1;
}

uint16_t ConnectWindow::getPort(){
    if(ui->portLine->text().isEmpty())
        return 0;

    return htons(ui->portLine->text().toInt());
}

void ConnectWindow::on_connectButton_clicked()
{
    if(!validateWindow())
        return;

    sockaddr_in l_sock_buff;
    memset(&l_sock_buff, 0, sizeof (l_sock_buff));


    if(!getWindowData(l_sock_buff))
        return;

    l_sock_buff.sin_family = AF_INET;

    g_connectionManager.createClient(l_sock_buff, ui->tcpButton->isChecked() ? SOCK_STREAM : SOCK_DGRAM);

    if(g_connectionManager.getConnectionState() != connectionStatus::ESTABLISHED){
        QMessageBox msg;
        if(g_connectionManager.getConnectionState() == connectionStatus::SOCKET_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to create socket!");
            return;
        }
        else if(g_connectionManager.getConnectionState() == connectionStatus::CONNECT_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to connect to the server!");
            return;
        }
    }

    m_chatWindow = std::make_unique<ChatWindow>();
    m_chatWindow->show();

    this->close();
}


void ConnectWindow::on_createServerButton_clicked()
{    
    if(!validateStreamType())
        return;

    sockaddr_in l_sock_buff;
    memset(&l_sock_buff, 0, sizeof (l_sock_buff));

    l_sock_buff.sin_family = AF_INET;
    l_sock_buff.sin_addr.s_addr = INADDR_ANY;
    l_sock_buff.sin_port = getPort();

    g_connectionManager.createServer(l_sock_buff, ui->tcpButton->isChecked() ? SOCK_STREAM : SOCK_DGRAM);

    if(g_connectionManager.getConnectionState() != connectionStatus::SERVER_READY){
        QMessageBox msg;
        if(g_connectionManager.getConnectionState() == connectionStatus::SOCKET_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to create socket!");
            return;
        }
        else if(g_connectionManager.getConnectionState() == connectionStatus::BIND_ERROR){
            QMessageBox::warning(&msg, "Error!", "Port is busy!");
            return;
        }
        else if(g_connectionManager.getConnectionState() == connectionStatus::LISTEN_ERROR){
            QMessageBox::warning(&msg, "Error!", "Failed to call listen()!");
            return;
        }
    }

    m_chatWindow = std::make_unique<ChatWindow>();
    m_chatWindow->show();

    this->close();
}
