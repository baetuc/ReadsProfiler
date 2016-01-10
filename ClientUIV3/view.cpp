#include "view.h"
#include "ui_view.h"
#include<Utility.h>
#include<QFileDialog>
#include<QMessageBox>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<QMessageBox>
#include<search.h>
#include<mainwindow.h>
#include<Rating.h>
#include<SerializerDeserializer.h>
#include<Reader.h>

#define FETCH 4
#define RATE 5

View::View(Book book, list<Book> books, string username, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    this->username = username;
    this->book = book;
    this->books = books;

    string loggedAs = "Logat ca: ";
    loggedAs += username;
    ui->logged_as->setText(loggedAs.c_str());

    Creation creation = book.getCreation();

    ui->title->setText(creation.getTitle().c_str());
    ui->volume->setText(Utility::getStringForNumber(creation.getVolume()).c_str());
    list<Author> authors = creation.getAuthors();
    string auths;
    for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
        auths += it->getSecondName();
        auths += " ";
        auths += it->getFirstName();
        auths += ", ";
    }
    auths = auths.substr(0, auths.size() - 2);
    ui->authors->setText(auths.c_str());


    list<string> genres = creation.getGenres();
    string gen;
    for(list<string>::iterator it = genres.begin(); it != genres.end(); ++it) {
        gen += *it;
        gen += ", ";
    }
    gen = gen.substr(0, gen.size() - 2);
    ui->genres->setText(gen.c_str());

    list<string> subgenres = creation.getSubgenres();
    string subgen;
    for(list<string>::iterator it = subgenres.begin(); it != subgenres.end(); ++it) {
        subgen += *it;
        subgen += ", ";
    }
    subgen = subgen.substr(0, subgen.size() - 2);
    ui->subgenres->setText(subgen.c_str());

    ui->publisher->setText(book.getPublisher().c_str());
    ui->isbn->setText(book.getISBN().c_str());
    ui->year->setText(Utility::getStringForNumber(book.getPublicationYear()).c_str());
    ui->rating->setText(Utility::getStringForFloat(book.getRating()).c_str());
    //ui->description->setText(book.getDescription().c_str());
    ui->progressBar->setVisible(false); // will be visible only when download in progress
    ui->downloading->setVisible(false);
}

View::~View()
{
    delete ui;
}

void View::on_toolButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Alegeți locația unde se va descărca cartea"), "/home",
                                                 "Toate extensiile (*.*);; Fișier text (*.txt)");
    ui->path->setText(filename);
}

void View::on_pushButton_clicked()
{
    if(ui->path->text().size() == 0) {
        QMessageBox::warning((QWidget*)this->parent(), "WARNING", "Nu ați specificat calea de descărcare a fișierului");
    }
    else {

        ui->pushButton->setVisible(false);
        ui->commandLinkButton->setVisible(false);
        ui->toolButton->setVisible(false);
        ui->pushButton_2->setVisible(false);
        ui->pushButton_3->setVisible(false);
        ui->spinBox->setVisible(false);
        ui->path->setVisible(false);
        ui->downloading->setVisible(true);

        uint32_t requestType = FETCH;
        requestType = htonl(requestType);
        if(write(sd, &requestType, sizeof(uint32_t)) < 0) {
            QMessageBox::warning(this, "WARNING", "Eroare la transmiterea cererii de descărcare la server!");
            exit(0);
        }

        Reader::sendMessageToServer(sd, (MainWindow*) this->parent(), this->book.getISBN());

        int to =  ::open(ui->path->text().toStdString().c_str(), O_TRUNC | O_CREAT | O_WRONLY, (mode_t)0700);
        if(to < 0) {
            QMessageBox::warning(this, "WARNING", "Eroare la crearea unui nou fișier!");
            exit(0);
        }

        // We start to download the file
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(true);


        uint32_t size;
        if(read(sd, &size, sizeof(uint32_t)) < 0) {
            QMessageBox::warning((QWidget*)this->parent(), "WARNING", "Eroare la citirea lungimii mesajului de la server!");
            exit(0);
        }

        size = ntohl(size);
        char letter;
        unsigned int percent = 0;

        for(uint32_t i = 0; i < size; ++i) {
            if (read(sd, &letter, sizeof(char)) <= 0) {
                QMessageBox::warning((QWidget*)this->parent(), "WARNING", "Eroare la citirea fișierului de la server!");
                exit(0);
            }
            if (write(to, &letter, sizeof(char)) <= 0) {
                QMessageBox::warning((QWidget*)this->parent(), "WARNING", "Eroare la scrierea informațiilor în fișier!");
                exit(0);
            }
            if((i * 100) / size > percent) {
                percent = (i * 100) / size;
                ui->progressBar->setValue(percent);
            }
        }
         QMessageBox::information(this, "Information", "Fișierul a fost descărcat!");

         ui->pushButton->setVisible(true);
         ui->commandLinkButton->setVisible(true);
         ui->toolButton->setVisible(true);
         ui->pushButton_2->setVisible(true);
         ui->pushButton_3->setVisible(true);
         ui->spinBox->setVisible(true);
         ui->path->setVisible(true);
         ui->progressBar->setVisible(false);
         ui->downloading->setVisible(false);
    }
}

void View::on_commandLinkButton_clicked()
{
    Search search(books, this->username);
    search.setVisible(true);
    this->setVisible(false);
    search.sd = this->sd;
    search.connected = this->connected;
    this->close();
    search.exec();
}

void View::on_pushButton_2_clicked()
{
    MainWindow* window = new MainWindow(sd, true, this->username);
    window->setVisible(true);
    this->setVisible(false);
    this->close();
    window->show();

}

void View::on_pushButton_3_clicked()
{
    uint32_t requestType = RATE;
    requestType = htonl(requestType);
    if(write(sd, &requestType, sizeof(uint32_t)) < 0) {
        QMessageBox::warning(this, "WARNING", "Eroare la transmiterea cererii de rating la server!");
        exit(0);
    }

    Rating rating;
    rating.setISBN(this->book.getISBN());
    rating.setRating(ui->spinBox->value());

    string serializedRating = SerializerDeserializer::serializeRating(rating);
    Reader::sendMessageToServer(sd, (MainWindow*)this->parent(), serializedRating);
    // Receive message that lets you know if you have the right to rate or not :D
}
