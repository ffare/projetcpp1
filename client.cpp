#include "client.h"

Client::Client(QString nom, QString prenom, QString email,
               QString telephone, QString adresse, QString ville)
{
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->telephone = telephone;
    this->adresse = adresse;
    this->ville = ville;
}

bool Client::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO CLIENT (nom, prenom, email, telephone, adresse, ville) "
                  "VALUES (:nom, :prenom, :email, :telephone, :adresse, :ville)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":adresse", adresse);
    query.bindValue(":ville", ville);

    if(query.exec())
    {
        qDebug() << "Client ajouté avec succès";
        return true;
    }
    else
    {
        qDebug() << "Erreur ajout :" << query.lastError().text();
        return false;
    }
}
