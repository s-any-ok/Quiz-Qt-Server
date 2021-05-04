#include <QCoreApplication>
#include "server.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DatabaseManager data;
    Server server(data);

//    data.setDB("D:/SQLiteDatabaseBrowserPortable/Data/", "database.db");
//    QJsonArray arr = data.getData();
//    server.sendData(arr);
    server.startServer();


    return a.exec();
}
