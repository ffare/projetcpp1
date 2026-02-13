#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection
{
public:
    static bool createConnection()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("logistique");
        db.setUserName("root");      // XAMPP default
        db.setPassword("");          // XAMPP default empty

        if (!db.open())
        {
            qDebug() << "Database error:" << db.lastError().text();
            return false;
        }

        qDebug() << "Database connected successfully!";
        return true;
    }
};

#endif
