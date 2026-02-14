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

private:
    Ui::MainWindow *ui;

    void loadClients(); // Function to load data into tableClient_1
};

#endif // MAINWINDOW_H
