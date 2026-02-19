#include "vehicule.h"

QString Vehicule::toInsertQuery() const
{
    return QString(
               "INSERT INTO vehicule "
               "(id, marque, modele, immatriculation, type, carburant, capacite, kilometrage, statut) "
               "VALUES (%1, '%2', '%3', '%4', '%5', '%6', %7, %8, '%9');"
               )
        .arg(id)
        .arg(marque)
        .arg(modele)
        .arg(immatriculation)
        .arg(type)
        .arg(carburant)
        .arg(capacite)
        .arg(kilometrage)
        .arg(statut);
}
