#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Client
{
private:
    QString nom, prenom, email, telephone, adresse, ville;

public:
    Client(QString nom, QString prenom, QString email,
           QString telephone, QString adresse, QString ville);

    bool ajouter();
};

#endif
