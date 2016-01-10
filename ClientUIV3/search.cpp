#include "search.h"
#include "ui_search.h"
#include<SerializerDeserializer.h>
#include<Book.h>
#include<Reader.h>
#include<list>
#include<Utility.h>
#include<QTableWidget>
#include<QHeaderView>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<view.h>
#include<mainwindow.h>
#include<dialog.h>

Search::Search(int sd, string username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    this->sd = sd;
    this->username = username;
    ui->setupUi(this);
    string loggedAs = "Logat ca: ";
    loggedAs += username;
    ui->logged_as->setText(loggedAs.c_str());

    // At first we receive info from the server, then we populate the table with data

    string serializedBooks = Reader::receiveMessageFromServer(sd, (MainWindow*)this->parent());
    list<Book> books = SerializerDeserializer::deserializeBookList(serializedBooks);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setVisible(true);
    ui->tableWidget->setHorizontalHeaderLabels(QString("ISBN;Titlu;Volum;Editura;Rating;Află mai multe").split(";"));

    int index = 0;
    for(list<Book>::iterator it = books.begin(); it != books.end(); ++it) {
        ui->tableWidget->insertRow(index);
        QWidget* isbn = new QLabel(QString(it->getISBN().c_str()));
        QWidget* title = new QLabel(it->getCreation().getTitle().c_str());
        string volumeToFill = it->getCreation().getVolume() == 0 ? "-" :
                              Utility::getStringForNumber(it->getCreation().getVolume());
        QWidget* volume = new QLabel(volumeToFill.c_str());
        QWidget* publisher = new QLabel(it->getPublisher().c_str());
        list<Author> authors = it->getCreation().getAuthors();
        Book book = *it;
        /*string auths;
        for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
            auths += it->getSecondName();
            auths += " ";
            auths += it->getFirstName();
            auths += ", ";
        }
        if(authors.size() > 0) {
            auths = auths.substr(0, auths.size() - 2);
        }
        QWidget* author = new QLabel(auths.c_str());
        */
        QWidget* rating = new QLabel(Utility::getStringForNumber(it->getRating()).c_str());
        QPushButton* pushButton = new QPushButton("Detalii");
        connect(pushButton,  &QPushButton::clicked, [=]{ viewButton(book, books); });

        ui->tableWidget->setCellWidget(index, 0, isbn);
        ui->tableWidget->setCellWidget(index, 1, title);
        ui->tableWidget->setCellWidget(index, 2, volume);
        ui->tableWidget->setCellWidget(index, 3, publisher);
        ui->tableWidget->setCellWidget(index, 4, rating);
        ui->tableWidget->setCellWidget(index, 5, pushButton);
        ++index;
    }

    ui->tableWidget->sortByColumn(4, Qt::DescendingOrder);
    // sorts table in descending order after rating
}

Search::Search(list<Book> books, string username, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Search) {

    this->books = books;
    this->username = username;
    ui->setupUi(this);
    string loggedAs = "Logat ca: ";
    loggedAs += username;
    ui->logged_as->setText(loggedAs.c_str());
    // At first we receive info from the server, then we populate the table with data

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setVisible(true);
    ui->tableWidget->setHorizontalHeaderLabels(QString("ISBN;Titlu;Volum;Editura;Rating;Află mai multe").split(";"));

    int index = 0;
    for(list<Book>::iterator it = books.begin(); it != books.end(); ++it) {
        ui->tableWidget->insertRow(index);
        QWidget* isbn = new QLabel(QString(it->getISBN().c_str()));
        QWidget* title = new QLabel(it->getCreation().getTitle().c_str());
        string volumeToFill = it->getCreation().getVolume() == 0 ? "-" :
                              Utility::getStringForNumber(it->getCreation().getVolume());
        QWidget* volume = new QLabel(volumeToFill.c_str());
        QWidget* publisher = new QLabel(it->getPublisher().c_str());
        list<Author> authors = it->getCreation().getAuthors();
        Book book = *it;
        /*string auths;
        for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
            auths += it->getSecondName();
            auths += " ";
            auths += it->getFirstName();
            auths += ", ";
        }
        if(authors.size() > 0) {
            auths = auths.substr(0, auths.size() - 2);
        }
        QWidget* author = new QLabel(auths.c_str());
        */
        QWidget* rating = new QLabel(Utility::getStringForNumber(it->getRating()).c_str());
        QPushButton* pushButton = new QPushButton("Detalii");
        connect(pushButton,  &QPushButton::clicked, [=]{ viewButton(book, books); });

        ui->tableWidget->setCellWidget(index, 0, isbn);
        ui->tableWidget->setCellWidget(index, 1, title);
        ui->tableWidget->setCellWidget(index, 2, volume);
        ui->tableWidget->setCellWidget(index, 3, publisher);
        ui->tableWidget->setCellWidget(index, 4, rating);
        ui->tableWidget->setCellWidget(index, 5, pushButton);
        ++index;
    }

    ui->tableWidget->sortByColumn(4, Qt::DescendingOrder);
}

Search::~Search()
{
    delete ui;
}

void Search::viewButton(Book book, list<Book> books) {
    View view(book, books, this->username);
    view.setVisible(true);
    this->setVisible(false);
    view.sd = this->sd;
    view.connected = this->connected;
    this->close();
    view.exec();
}

void Search::on_commandLinkButton_clicked() {
    Dialog dialog(this->username);
    dialog.setVisible(true);
    this->setVisible(false);
    dialog.sd = this->sd;
    dialog.connected = this->connected;
    this->close();
    dialog.exec();
}

void Search::on_pushButton_clicked()
{

    MainWindow* window = new MainWindow(sd, true, this->username);
    window->setVisible(true);
    this->setVisible(false);
    window->sd = this->sd;
    window->connected = this->connected;
    this->close();
    window->show();
}
