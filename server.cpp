#include "server.h"


Server::Server(DatabaseManager dataManager)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    _dataManager = dataManager;
    _dataManager.setDB("D:/SQLiteDatabaseBrowserPortable/Data/", "database.db");
}

Server::~Server()
{

}

void Server::startServer()
{
    if (this->listen(QHostAddress::Any,5555))
    {
        qDebug()<<"Listening";
    }
    else
    {
        qDebug()<<"Not listening";
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
}

void Server::sockReady()
{

    Data = socket->readAll();
    qDebug() << Data;
    if (Data == "get") {
        string stdString = toSerialiseString(_dataManager.getData());
        QByteArray byteArray(stdString.c_str(), stdString.length());
        socket->write(byteArray);
    } else if (Data.at(0) == 'r') {
        string dataStr(Data.constData(), Data.length());
        _dataManager.setData(dataStr.substr(1, dataStr.size()));
    }
}

void Server::sockDisc()
{
    qDebug()<<"Disconnect";
    socket->deleteLater();
}

string Server::toSerialiseString(QJsonArray data)
{
    string questions;
    foreach (const QJsonValue & v, data) {
        int id = v.toObject().value("id").toInt();
        string question = v.toObject().value("question").toString().toLocal8Bit().constData();
        int correctAnswerId = v.toObject().value("correctAnswer").toInt();

        QJsonArray answers =  v.toObject().value("answers").toArray();
        vector<string> answersList;
        foreach (const QJsonValue & v, answers) {
            string answer = v.toString().toLocal8Bit().constData();
            answersList.push_back(answer);
        }
        string serialiseString = "";
        serialiseString += "--(id)--" + to_string(id);
        serialiseString += "--(correct answer)--" + to_string(correctAnswerId);
        serialiseString += "--(question)--" + question;
        for (int i = 0; i < 4; i++)
        {
            serialiseString += "--(answer " + to_string(i + 1) + ")--" + answersList[i];
        }

        questions += serialiseString;
        questions += "$$$";
    }

    return questions;
}
