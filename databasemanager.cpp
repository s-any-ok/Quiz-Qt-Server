#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QSqlError>
#include <string>
#include "databasemanager.h"


DatabaseManager::DatabaseManager()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
}

void DatabaseManager::setDB(QString databasePath, QString databaseName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath + databaseName);
}

DatabaseManager::~DatabaseManager(){}

void DatabaseManager::Connect()
{
    db.open();
}


void DatabaseManager::Disconnect()
{
    db.close();
}

QJsonArray DatabaseManager::getData()
  {
    QJsonArray array;
    Connect();

    QSqlQuery query;
    QSqlQuery query2;
    query.exec("SELECT id, QuestionText FROM Question");

    auto question = QJsonObject();
    int i = 0;
    while (query.next())
    {
        auto answers = QJsonArray();
        QString id = query.value(0).toString();
        QString QuestionText = query.value(1).toString();
        QString IsCorrect;
        query2.exec("SELECT * FROM Answer where QuestionId = " + id);

        while (query2.next())
        {
            QString AnswerText = query2.value(1).toString();
            if (query2.value(2).toString() == "1") IsCorrect = query2.value(0).toString();

            answers.push_back(AnswerText);
        }

        question = QJsonObject(
        {
            qMakePair(QString("id"), id.toInt()),
            qMakePair(QString("question"), QuestionText),
            qMakePair(QString("correctAnswer"), IsCorrect.toInt() - i),
            qMakePair(QString("answers"), answers),
        });
        array.push_back(question);
        i += 4;
    }
    Disconnect();
    return array;
  }

void DatabaseManager::setData(string result)
  {
    Connect();
    QString qresult = QString::fromStdString(result);
    QSqlQuery query;
        query.prepare("INSERT INTO result(date, result) VALUES (:date, :result);");
        query.bindValue(":result", qresult.toInt());
        query.bindValue(":date", QDateTime::currentDateTime().toString());
        query.exec();
  }
