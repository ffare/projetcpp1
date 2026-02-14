#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class Connection
{
public:
    // Get the database connection
    static QSqlDatabase& getDatabase()
    {
        static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        return db;
    }

    // Create/open the connection
    static bool createConnection()
    {
        QSqlDatabase &db = getDatabase();

        if (db.isOpen())
        {
            qDebug() << "Database already open!";
            return true;
        }

        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("logistique");
        db.setUserName("root");  // XAMPP default
        db.setPassword("");      // XAMPP default empty

        if (!db.open())
        {
            qDebug() << "Database connection failed:" << db.lastError().text();
            return false;
        }

        qDebug() << "Database connected successfully!";
        return true;
    }

    // Close the connection (optional, good practice)
    static void closeConnection()
    {
        QSqlDatabase &db = getDatabase();
        if (db.isOpen())
            db.close();
        QSqlDatabase::removeDatabase("qt_sql_default_connection");
        qDebug() << "Database connection closed.";
    }
};

#endif // CONNECTION_H
