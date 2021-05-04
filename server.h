#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextCodec>
#include <QSqlDatabase>
#include "QSqlQuery"
#include <QJsonObject>
#include <string>
#include "databasemanager.h"
using namespace std;


class Server: public QTcpServer
{
    Q_OBJECT
private:
    DatabaseManager _dataManager;

public:
    Server(DatabaseManager);
    ~Server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(qintptr  socketDescriptor);
    void sockReady();
    void sockDisc();

    string toSerialiseString(QJsonArray data);
};

#endif // SERVER_H

