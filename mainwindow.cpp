#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QVBoxLayout>
#include <QSqlRecord>
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTextEdit>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableClient_1->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableClient_1->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableVehicule_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableVehicule_2->setSelectionMode(QAbstractItemView::SingleSelection);

    // Connect buttons
    // connect(ui->pd_ajouter_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_1_clicked);
    // connect(ui->pd_ajouter_2_1, &QPushButton::clicked, this, &MainWindow::on_pd_ajouter_2_1_clicked);
    // connect(ui->delete_btn_1, &QPushButton::clicked, this, &MainWindow::on_delete_btn_1_clicked);

    // Open database connection
    if (!Connection::createConnection()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database!");
        return;
    }

    // Load when app starts
    loadClients();
    loadVehicules();
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


void MainWindow::on_rechercherText_1_textChanged(const QString &text)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    QString queryStr =
        "SELECT id, nom, prenom, cin, email, telephone, profession, role, date_naissance "
        "FROM client "
        "WHERE nom LIKE '%" + text + "%' "
                 "OR prenom LIKE '%" + text + "%' "
                 "OR cin LIKE '%" + text + "%'";

    model->setQuery(queryStr);

    ui->tableClient_1->setModel(model);
}

void MainWindow::on_pdf_1_clicked()
{
    // Get selected row
    QModelIndexList selection = ui->tableClient_1->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un client à imprimer !");
        return;
    }

    int row = selection.at(0).row();
    QAbstractItemModel *model = ui->tableClient_1->model();
    const int columnCount = model->columnCount();

    // Start HTML
    QString strStream;
    QTextStream out(&strStream);
    out << "<html>\n<head>\n"
        << "<meta charset='UTF-8'>\n"
        << "<title>Client PDF</title>\n"
        << "<style>"
        << "table { border-collapse: collapse; width: 100%; }"
        << "th, td { border: 1px solid black; padding: 4px; text-align: left; }"
        << "th { background-color: #f0f0f0; }"
        << "</style>\n"
        << "</head>\n<body>\n"
        << "<h1>Informations du client</h1>\n"
        << "<table>\n";

    // Table headers and values
    out << "<tr>";
    for (int col = 0; col < columnCount; ++col) {
        if (!ui->tableClient_1->isColumnHidden(col)) {
            out << QString("<th>%1</th>").arg(model->headerData(col, Qt::Horizontal).toString());
        }
    }
    out << "</tr>\n<tr>";

    for (int col = 0; col < columnCount; ++col) {
        if (!ui->tableClient_1->isColumnHidden(col)) {
            QString data = model->data(model->index(row, col)).toString().simplified();
            out << QString("<td>%1</td>").arg(!data.isEmpty() ? data : "&nbsp;");
        }
    }
    out << "</tr>\n";

    out << "</table>\n</body>\n</html>";

    // Create QTextDocument
    QTextDocument document;
    document.setHtml(strStream);

    // Ask user where to save PDF
    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "client.pdf", "*.pdf");
    if (filename.isEmpty()) return;

    // Print to PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    document.print(&printer);

    QMessageBox::information(this, "Succès", "PDF du client créé avec succès !");
}

void MainWindow::on_sms_1_clicked()
{


}

void MainWindow::on_pushButton_2_2_clicked()
{
    // Gmail SMTP info
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465; // SSL port
    QString username = "faresbenlakdher@gmail.com";
    QString password = "madc qjef sptt tecu";
    QString recipient   = ui->recvr_1->text().trimmed();
    QString subject     = ui->subject_1->text().trimmed();
    QString messageBody = ui->mail_1->text().trimmed();

    QSslSocket socket;
    socket.connectToHostEncrypted(smtpServer, smtpPort);

    if (!socket.waitForEncrypted(5000)) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter au serveur SMTP !");
        return;
    }

    // Compose a simple SMTP conversation (base64 encoding required for AUTH)
    auto sendLine = [&socket](const QString &line){
        socket.write((line + "\r\n").toUtf8());
        socket.flush();
        socket.waitForReadyRead(2000);
    };

    sendLine("EHLO localhost");
    sendLine("AUTH LOGIN");
    sendLine(username.toUtf8().toBase64());
    sendLine(password.toUtf8().toBase64());
    sendLine("MAIL FROM:<" + username + ">");
    sendLine("RCPT TO:<" + recipient + ">");
    sendLine("DATA");
    sendLine("Subject: " + subject);
    sendLine("From: " + username);
    sendLine("To: " + recipient);
    sendLine(""); // blank line before message body
    sendLine(messageBody);
    sendLine("."); // end of message
    sendLine("QUIT");

    socket.disconnectFromHost();

    QMessageBox::information(this, "Info", "Email envoyé !");
}


void MainWindow::on_pushButton_3_1_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistique des Clients par Rôle");
    dialog->resize(800, 500);

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database NOT connected!";
        return;
    }

    QSqlQuery query;

    // This query groups clients by role automatically
    if(!query.exec("SELECT role, COUNT(*) FROM client GROUP BY role"))
    {
        qDebug() << "Query error:" << query.lastError();
        return;
    }

    QBarSet *set = new QBarSet("Nombre de Clients");
    QStringList categories;

    while(query.next())
    {
        QString role = query.value(0).toString();
        int count = query.value(1).toInt();

        categories << role;
        *set << count;
    }

    if(categories.isEmpty())
    {
        QMessageBox::warning(this, "Info", "Aucune donnée trouvée !");
        return;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Clients par Rôle");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de Clients");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    dialog->show();
}

void MainWindow::on_commandLinkButton_25_clicked()
{

}


void MainWindow::on_commandLinkButton_18_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_commandLinkButton_24_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


// ----------------Vehicule---------------------

void MainWindow::loadVehicules()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT id, marque, modele, immatriculation, type, carburant, capacite, kilometrage, statut FROM vehicule");

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Marque");
    model->setHeaderData(2, Qt::Horizontal, "Modele");
    model->setHeaderData(3, Qt::Horizontal, "Immatriculation");
    model->setHeaderData(4, Qt::Horizontal, "Type");
    model->setHeaderData(5, Qt::Horizontal, "Carburant");
    model->setHeaderData(6, Qt::Horizontal, "Capacité");
    model->setHeaderData(7, Qt::Horizontal, "Kilometrage");
    model->setHeaderData(8, Qt::Horizontal, "Statut");

    ui->tableVehicule_2->setModel(model);
    ui->tableVehicule_2->resizeColumnsToContents();
}

void MainWindow::on_ajouter_3_clicked()
{
    QSqlQuery query;

    query.prepare("INSERT INTO vehicule "
                  "(id, marque, modele, immatriculation, type, carburant, capacite, kilometrage, statut) "
                  "VALUES (:id, :marque, :modele, :immatriculation, :type, :carburant, :capacite, :kilometrage, :statut)");

    query.bindValue(":id", ui->id_3->text().toInt());
    query.bindValue(":marque", ui->marque_3->text());
    query.bindValue(":modele", ui->modele_3->text());
    query.bindValue(":immatriculation", ui->immatriculation_3->text());
    query.bindValue(":type", ui->type_3->text());
    query.bindValue(":carburant", ui->carburant_3->text());
    query.bindValue(":capacite", ui->capacite_3->text().toInt());
    query.bindValue(":kilometrage", ui->kilometrage_3->text().toInt());
    query.bindValue(":statut", ui->statut_3->text());

    if(query.exec())
    {
        QMessageBox::information(this, "Succès", "Véhicule ajouté !");
        loadVehicules();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}



void MainWindow::on_modifier_3_clicked()
{
    QSqlQuery query;

    query.prepare("UPDATE vehicule SET "
                  "marque=:marque, modele=:modele, immatriculation=:immatriculation, "
                  "type=:type, carburant=:carburant, capacite=:capacite, "
                  "kilometrage=:kilometrage, statut=:statut "
                  "WHERE id=:id");

    query.bindValue(":id", ui->id_3->text().toInt());
    query.bindValue(":marque", ui->marque_3->text());
    query.bindValue(":modele", ui->modele_3->text());
    query.bindValue(":immatriculation", ui->immatriculation_3->text());
    query.bindValue(":type", ui->type_3->text());
    query.bindValue(":carburant", ui->carburant_3->text());
    query.bindValue(":capacite", ui->capacite_3->text().toInt());
    query.bindValue(":kilometrage", ui->kilometrage_3->text().toInt());
    query.bindValue(":statut", ui->statut_3->text());

    if(query.exec())
    {
        QMessageBox::information(this, "Succès", "Véhicule modifié !");
        loadVehicules();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}


void MainWindow::on_delete_btn_2_clicked()
{
    QModelIndexList selection = ui->tableVehicule_2->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un véhicule !");
        return;
    }

    int row = selection.at(0).row();
    int vehiculeId = ui->tableVehicule_2->model()->index(row, 0).data().toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM vehicule WHERE id = :id");
    query.bindValue(":id", vehiculeId);

    if(query.exec())
    {
        QMessageBox::information(this, "Succès", "Véhicule supprimé !");
        loadVehicules();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
    }
}


void MainWindow::on_pushButton_3_2_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistique des Véhicules par Statut");
    dialog->resize(800, 500);

    if (!QSqlDatabase::database().isOpen()) {
        qDebug() << "Database NOT connected!";
        return;
    }

    QSqlQuery query;

    // Group vehicules by statut
    if(!query.exec("SELECT statut, COUNT(*) FROM vehicule GROUP BY statut"))
    {
        qDebug() << "Query error:" << query.lastError();
        return;
    }

    QBarSet *set = new QBarSet("Nombre de Véhicules");
    QStringList categories;

    while(query.next())
    {
        QString statut = query.value(0).toString();
        int count = query.value(1).toInt();

        categories << statut;
        *set << count;
    }

    if(categories.isEmpty())
    {
        QMessageBox::warning(this, "Info", "Aucune donnée trouvée !");
        return;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Véhicules par Statut");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de Véhicules");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    dialog->show();
}


void MainWindow::on_pdf_2_clicked()
{
    // Get selected row
    QModelIndexList selection = ui->tableVehicule_2->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un client à imprimer !");
        return;
    }

    int row = selection.at(0).row();
    QAbstractItemModel *model = ui->tableVehicule_2->model();
    const int columnCount = model->columnCount();

    // Start HTML
    QString strStream;
    QTextStream out(&strStream);
    out << "<html>\n<head>\n"
        << "<meta charset='UTF-8'>\n"
        << "<title>Vehicule PDF</title>\n"
        << "<style>"
        << "table { border-collapse: collapse; width: 100%; }"
        << "th, td { border: 1px solid black; padding: 4px; text-align: left; }"
        << "th { background-color: #f0f0f0; }"
        << "</style>\n"
        << "</head>\n<body>\n"
        << "<h1>Informations du vehicule</h1>\n"
        << "<table>\n";

    // Table headers and values
    out << "<tr>";
    for (int col = 0; col < columnCount; ++col) {
        if (!ui->tableVehicule_2->isColumnHidden(col)) {
            out << QString("<th>%1</th>").arg(model->headerData(col, Qt::Horizontal).toString());
        }
    }
    out << "</tr>\n<tr>";

    for (int col = 0; col < columnCount; ++col) {
        if (!ui->tableVehicule_2->isColumnHidden(col)) {
            QString data = model->data(model->index(row, col)).toString().simplified();
            out << QString("<td>%1</td>").arg(!data.isEmpty() ? data : "&nbsp;");
        }
    }
    out << "</tr>\n";

    out << "</table>\n</body>\n</html>";

    // Create QTextDocument
    QTextDocument document;
    document.setHtml(strStream);

    // Ask user where to save PDF
    QString filename = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "vehicule.pdf", "*.pdf");
    if (filename.isEmpty()) return;

    // Print to PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    document.print(&printer);

    QMessageBox::information(this, "Succès", "PDF du vehicule créé avec succès !");
}


void MainWindow::on_rechercherText_2_textChanged(const QString &text)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);

    QString queryStr =
        "SELECT id, marque, modele, immatriculation, type, carburant, capacite, kilometrage, statut "
        "FROM vehicule "
        "WHERE marque LIKE '%" + text + "%' "
                 "OR modele LIKE '%" + text + "%' "
                 "OR immatriculation LIKE '%" + text + "%' "
                 "OR statut LIKE '%" + text + "%'";

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "Search error:" << model->lastError();
    }

    ui->tableVehicule_2->setModel(model);

}

