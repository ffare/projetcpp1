#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!Connection::createConnection())
        return -1;

    MainWindow w;
    // qDebug() << QSqlDatabase::drivers();
    w.show();
    return a.exec();
}
