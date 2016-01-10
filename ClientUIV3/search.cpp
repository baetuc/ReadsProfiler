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

Search::Search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    // At first we receive info from the server, then we populate the table with data

    string serializedBooks = Reader::receiveMessageFromServer(sd, (MainWindow*)this->parent());
    list<Book> books = SerializerDeserializer::deserializeBookList(serializedBooks);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setVisible(false);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setVisible(true);
    ui->tableWidget->setHorizontalHeaderLabels(QString("ISBN;Titlu;Rating;Află mai multe").split(";"));

    int index = 0;
    for(list<Book>::iterator it = books.begin(); it != books.end(); ++it) {
        ui->tableWidget->insertRow(index);
        QWidget* isbn = new QLabel(QString(it->getISBN().c_str()));
        QWidget* title = new QLabel(it->getCreation().getTitle().c_str());
        QWidget* rating = new QLabel(Utility::getStringForNumber(it->getRating()).c_str());
        QPushButton* pushButton = new QPushButton("Detalii");
        connect(pushButton, SIGNAL(clicked()), this, SLOT(viewButton(book)));
        ui->tableWidget->setCellWidget(index, 0, isbn);
        ui->tableWidget->setCellWidget(index, 1, title);
        ui->tableWidget->setCellWidget(index, 2, rating);
        ui->tableWidget->setCellWidget(index, 3, pushButton);
        ++index;
    }

    ui->tableWidget->sortByColumn(3, Qt::DescendingOrder);
    // sorts table by descending table
}

Search::~Search()
{
    delete ui;
}

void Search::viewButton(Book book) {
    View view;
    view.setVisible(true);
    this->setVisible(false);
    view.sd = this->sd;
    view.connected = this->connected;
    view.book = book;
    this->close();
    view.exec();
}

void Search::on_commandLinkButton_clicked() {
    Dialog dialog;
    dialog.setVisible(true);
    this->setVisible(false);
    dialog.sd = this->sd;
    dialog.connected = this->connected;
    this->close();
    dialog.exec();
}

void Search::on_pushButton_clicked()
{
    MainWindow window;
    window.setVisible(true);
    this->setVisible(false);
    window.sd = this->sd;
    window.connected = this->connected;
    this->close();
    window.show();
}
