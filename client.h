#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

class Client
{
public:
    Client() = default;
    int id;
    QString nom;
    QString prenom;
    QString cin;
    QString email;
    QString telephone;
    QString profession;
    QString role;
    QString dateNaissance;
};

#endif // CLIENT_H
