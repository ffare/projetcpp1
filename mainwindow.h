#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pd_ajouter_1_clicked();   // Ajouter client
    void on_pd_ajouter_2_1_clicked(); // Modifier client
    void on_delete_btn_1_clicked();   // Supprimer client

    void on_centralwidget_customContextMenuRequested(const QPoint &pos);

    void on_tableClient_1_clicked(const QModelIndex &index);

    void on_rechercherText_1_textChanged(const QString &arg1);

    void on_pdf_1_clicked();

    void on_sms_1_clicked();

    void on_pushButton_2_2_clicked();

    void on_pushButton_3_1_clicked();

    void on_commandLinkButton_25_clicked();

    void on_commandLinkButton_18_clicked();

    void on_commandLinkButton_24_clicked();

    void on_ajouter_3_clicked();

    void on_modifier_3_clicked();

    void on_delete_btn_2_clicked();

    void on_pushButton_3_2_clicked();

private:
    Ui::MainWindow *ui;

    void loadClients(); // Function to load data into tableClient_1
    void loadVehicules();
};

#endif // MAINWINDOW_H
