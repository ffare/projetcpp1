#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableClient_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableClient_1->setSelectionMode(QAbstractItemView::SingleSelection);

    // Connect buttons
    // connect(ui->pd_ajouter_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_1_clicked);
    // connect(ui->pd_ajouter_2_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_2_1_clicked);
    // connect(ui->delete_btn_1, &QPushButton::clicked, this, &MainWindow::on_delete_btn_1_clicked);

    // Open database connection
    if (!Connection::createConnection()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database!");
        return;
    }

    // Load clients into table
    loadClients();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------- Add Client ----------------------
void MainWindow::on_pd_ajouter_1_clicked()
{
    Client c;
    c.id = ui->leid_1->text().toInt();
    c.nom = ui->lenom_1->text();
    c.prenom = ui->leprenom_1->text();
    c.cin = ui->lecin_1->text();
    c.email = ui->leemail_1->text();
    c.telephone = ui->letelephone_1->text();
    c.profession = ui->leprofeesion_1->text();
    c.role = ui->lerole_1->text();
    c.dateNaissance = ui->ledatenaissance_1->text();

    QSqlQuery query;
    query.prepare("INSERT INTO client (id, nom, prenom, cin, email, telephone, profession, role, date_naissance) "
                  "VALUES (:id, :nom, :prenom, :cin, :email, :telephone, :profession, :role, :date_naissance)");
    query.bindValue(":id", c.id);
    query.bindValue(":nom", c.nom);
    query.bindValue(":prenom", c.prenom);
    query.bindValue(":cin", c.cin);
    query.bindValue(":email", c.email);
    query.bindValue(":telephone", c.telephone);
    query.bindValue(":profession", c.profession);
    query.bindValue(":role", c.role);
    query.bindValue(":date_naissance", c.dateNaissance);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Client ajouté !");
        loadClients();
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

// ---------------------- Update Client ----------------------
void MainWindow::on_tableClient_1_clicked(const QModelIndex &index) // Add to table when selected
{
    if (!index.isValid())
        return;

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tableClient_1->model());
    if (!model)
        return;

    QSqlRecord record = model->record(index.row());

    ui->leid_1->setText(record.value("id").toString());
    ui->lenom_1->setText(record.value("nom").toString());
    ui->leprenom_1->setText(record.value("prenom").toString());
    ui->lecin_1->setText(record.value("cin").toString());
    ui->leemail_1->setText(record.value("email").toString());
    ui->letelephone_1->setText(record.value("telephone").toString());
    ui->leprofeesion_1->setText(record.value("profession").toString());
    ui->lerole_1->setText(record.value("role").toString());

    // Convert MySQL yyyy-MM-dd -> dd/MM/yyyy for display
    QDate date = QDate::fromString(record.value("date_naissance").toString(), "yyyy-MM-dd");
    ui->ledatenaissance_1->setText(date.toString("dd/MM/yyyy"));
}

void MainWindow::on_pd_ajouter_2_1_clicked()
{
    int id = ui->leid_1->text().toInt();

    QSqlQuery query;
    query.prepare("UPDATE client SET nom=:nom, prenom=:prenom, cin=:cin, email=:email, "
                  "telephone=:telephone, profession=:profession, role=:role, date_naissance=:date_naissance "
                  "WHERE id=:id");

    query.bindValue(":nom", ui->lenom_1->text());
    query.bindValue(":prenom", ui->leprenom_1->text());
    query.bindValue(":cin", ui->lecin_1->text());
    query.bindValue(":email", ui->leemail_1->text());
    query.bindValue(":telephone", ui->letelephone_1->text());
    query.bindValue(":profession", ui->leprofeesion_1->text());
    query.bindValue(":role", ui->lerole_1->text());

    // Convert date format for MySQL: yyyy-MM-dd
    QDate date = QDate::fromString(ui->ledatenaissance_1->text(), "dd/MM/yyyy");
    query.bindValue(":date_naissance", date.toString("yyyy-MM-dd"));

    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Info", "Client modifié !");
    loadClients();
}


// ---------------------- Delete Client ----------------------
void MainWindow::on_delete_btn_1_clicked()
{
    // Get selected row
    QModelIndexList selection = ui->tableClient_1->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Veuillez sélectionner un client à supprimer !");
        return;
    }

    int row = selection.at(0).row();

    // Get the client id from the first column (assuming id is column 0)
    int clientId = ui->tableClient_1->model()->index(row, 0).data().toInt();

    // Delete from database
    QSqlQuery query;
    query.prepare("DELETE FROM client WHERE id = :id");
    query.bindValue(":id", clientId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer le client:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Info", "Client supprimé !");
    loadClients(); // Refresh the table
}

// ---------------------- Load Clients ----------------------
void MainWindow::loadClients()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id, nom, prenom, cin, email, telephone, profession, role, date_naissance FROM client");

    // Optional: rename headers
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prenom");
    model->setHeaderData(3, Qt::Horizontal, "CIN");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Telephone");
    model->setHeaderData(6, Qt::Horizontal, "Profession");
    model->setHeaderData(7, Qt::Horizontal, "Role");
    model->setHeaderData(8, Qt::Horizontal, "Date Naissance");

    ui->tableClient_1->setModel(model);
    ui->tableClient_1->resizeColumnsToContents();
}

// ---------------------- Optional: Context Menu ----------------------
void MainWindow::on_centralwidget_customContextMenuRequested(const QPoint &pos)
{
    // Optional: implement context menu if needed
}
