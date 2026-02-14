#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>

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
    QString dateNaissance; // store as string "YYYY-MM-DD" for simplicity

    // Optional: helper to convert to/from SQL-ready strings
    QString toInsertQuery() const;
};

#endif // CLIENT_H
