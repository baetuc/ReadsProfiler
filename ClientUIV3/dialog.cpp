#include"dialog.h"
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"ui_dialog.h"
#include<Book.h>
#include<SerializerDeserializer.h>
#include<Utility.h>
#include<Reader.h>
#include<QMessageBox>
#include<search.h>

#define SEARCH 3

using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_cauta_clicked() {

       Book book;
       Creation creation;
       creation.setTitle(ui->titlu->text().toStdString());
       list<Author> authors;
       string auths = ui->autori->text().toStdString();
       vector<string> completeAuthors = Utility::splitString(auths, ',', 0);
       for(int i = 0; i < completeAuthors.size(); ++i) {
           vector<string> splitted = Utility::splitString(completeAuthors[i], ' ', 2);
           Author author;
           author.setSecondName(splitted[0]);
           author.setFirstName(splitted[1]);
           authors.push_back(author);
       }
       creation.setAuthors(authors);
       if(ui->volum->text().toStdString().size() == 0) {
           creation.setVolume(0);
       } else {
           creation.setVolume(atoi(ui->volum->text().toStdString().c_str()));
       }
       list<string> genres;
       vector<string> genress= Utility::splitString(ui->genuri->text().toStdString(), ',', 0);
       for(int i = 0; i < genress.size(); ++i) {
           genres.push_back(genress[i]);
       }
       creation.setGenres(genres);
       creation.setTitle(ui->titlu->text().toStdString());
       book.setCreation(creation);
       book.setISBN(ui->isbn->text().toStdString());
       book.setPublicationYear(atoi(ui->an->text().toStdString().c_str()));
       book.setPublisher(ui->editura->text().toStdString());
       book.setRating(atoi(ui->rating->text().toStdString().c_str()));

       string serializedBook = SerializerDeserializer::serializeBook(book);

       uint32_t requestType = SEARCH;
       requestType = htonl(requestType);
       if(write(sd, &requestType, sizeof(uint32_t)) < 0) {
           QMessageBox::warning(this, "WARNING", "Eroare la transmiterea cererii de search la server!");
           exit(0);
       }
       Reader::sendMessageToServer(sd, (MainWindow*) this->parent(), serializedBook);
       Search search;
       search.setVisible(true);
       this->setVisible(false);
       search.sd = this->sd;
       search.connected = this->connected;
       this->close();
       search.exec();
}

void Dialog::on_pushButton_clicked()
{
    MainWindow window;
    window.setVisible(true);
    this->setVisible(false);
    window.sd = this->sd;
    window.connected = this->connected;
    this->close();
    window.show();
}
