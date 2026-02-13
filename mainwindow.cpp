#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// void MainWindow::on_btn_ajouter_clicked()
// {
//     QString nom = ui->lineEdit_nom->text();
//     QString prenom = ui->lineEdit_prenom->text();
//     QString email = ui->lineEdit_email->text();
//     QString tel = ui->lineEdit_tel->text();
//     QString adresse = ui->lineEdit_adresse->text();
//     QString ville = ui->lineEdit_ville->text();

//     Client c(nom, prenom, email, tel, adresse, ville);

//     if(c.ajouter())
//         QMessageBox::information(this, "Success", "Client ajouté !");
//     else
//         QMessageBox::critical(this, "Error", "Erreur ajout !");
// }
