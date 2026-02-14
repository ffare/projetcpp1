#include "client.h"

QString Client::toInsertQuery() const
{
    return QString("(%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
    .arg(id)
        .arg(nom)
        .arg(prenom)
        .arg(cin)
        .arg(email)
        .arg(telephone)
        .arg(profession)
        .arg(role)
        .arg(dateNaissance);
}
