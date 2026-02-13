#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Example: connect signals to slots (auto if you name properly in Designer)
    connect(ui->pd_ajouter_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_1_clicked);
    connect(ui->pd_ajouter_2_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_2_1_clicked);
    connect(ui->delete_btn_1, &QPushButton::clicked, this, &MainWindow::on_delete_btn_1_clicked);

    loadClients();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------- Slots ----------------------------
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

    // TODO: save client to DB or internal list
    QMessageBox::information(this, "Info", "Client ajouté !");
    loadClients();
}

void MainWindow::on_pd_ajouter_2_1_clicked()
{
    // TODO: retrieve selected client and update
    QMessageBox::information(this, "Info", "Client modifié !");
    loadClients();
}

void MainWindow::on_delete_btn_1_clicked()
{
    // TODO: delete client
    QMessageBox::warning(this, "Info", "Client supprimé !");
    loadClients();
}

// ---------------------- Helper --------------------------
void MainWindow::loadClients()
{
    // TODO: load data into ui->tableClient_1
    // Example: clear and fill QTableView with QStandardItemModel
}

void MainWindow::on_centralwidget_customContextMenuRequested(const QPoint &pos)
{

}

