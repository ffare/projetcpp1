#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>

class Vehicule
{
public:
    Vehicule() = default;

    int id;
    QString marque;
    QString modele;
    QString immatriculation;
    QString type;              // Truck, Car, Van...
    QString carburant;         // Diesel, Essence...
    int capacite;              // Load capacity in kg
    int kilometrage;           // Current mileage
    QString statut;            // Disponible / En mission / Maintenance

    // Optional: helper to convert to SQL insert query
    QString toInsertQuery() const;
};

#endif // VEHICULE_H
