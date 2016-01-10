#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <User.h>
#include <SerializerDeserializer.h>
#include<iostream>
#include <QMessageBox>
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<dialog.h>
#include<Reader.h>

#define CONNECT 1
#define CREATE_USER 2

#define USER_PASSWORD_OK 'T'
#define WRONG_PASSWORD 'P'
#define WRONG_USERNAME 'U'

#define USER_EXISTS 'T'
#define USER_NO_EXISTS 'F'

using namespace std;

MainWindow::MainWindow(int sd, bool connected, string username, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->sd = sd;
    this->connected = connected;
    this->username = username;
    ui->setupUi(this);
    ui->lineEdit->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);

    ui->label_3->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
    ui->label_6->setVisible(false);

    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    try {
        this->prepareCommunication(); // connects to the server and initializes sd
    }
    catch(const char* message) {
        QMessageBox::warning(this, "WARNING", message);
        exit(0);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().size() == 0) {
        QMessageBox::warning(this, "WARNING", "Vă rugăm introduceți username-ul!");
    }
    else if (ui->lineEdit_2->text().size() == 0) {
        QMessageBox::warning(this, "WARNING", "Vă rugăm introduceți parola!");
    }
    else {
        uint32_t requestType = CONNECT;
        requestType = htonl(requestType);
        if(write(sd, &requestType, sizeof(uint32_t)) < 0) {
            QMessageBox::warning(this, "WARNING", "Eroare la transmiterea cererii de login la server!");
            exit(0);
        }
        User user;
        user.setUsername(ui->lineEdit->text().toStdString());
        user.setPassword(ui->lineEdit_2->text().toStdString());
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        string serializedUser = SerializerDeserializer::serializeUser(user);
        Reader::sendMessageToServer(sd, this, serializedUser);
        string response = Reader::receiveMessageFromServer(sd, this);

        switch(response[0]) {
            case WRONG_PASSWORD:
                        QMessageBox::warning(this, "WARNING", "Parola introdusă eronat!");
                        return;
            case WRONG_USERNAME:
                        QMessageBox::warning(this, "WARNING", "Nu există utilizatori cu username-ul specificat!");
                        return;
            case USER_PASSWORD_OK :
                        QMessageBox::information(this, "SUCCESS", "Loginul a avut loc cu succes!");
                        this->username = user.getUsername();
                        Dialog l(this->username);
                        l.setVisible(true);
                        this->setVisible(false);
                        l.sd = this->sd;
                        l.connected = this->connected;
                        this->close();
                        l.exec();

        }

    }
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    ui->lineEdit->setVisible(checked);
    ui->lineEdit_2->setVisible(checked);
    ui->label_3->setVisible(checked);
    ui->label_4->setVisible(checked);
    ui->pushButton->setVisible(checked);
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    ui->lineEdit_3->setVisible(checked);
    ui->lineEdit_4->setVisible(checked);
    ui->label_5->setVisible(checked);
    ui->label_6->setVisible(checked);
    ui->pushButton_2->setVisible(checked);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->lineEdit_3->text().size() == 0) {
        QMessageBox::warning(this, "WARNING", "Vă rugăm introduceți numele de utilizator!");
    }
    else if (ui->lineEdit_4->text().size() == 0) {
        QMessageBox::warning(this, "WARNING", "Vă rugăm introduceți parola!");
    }
    else {
        uint32_t requestType = CREATE_USER;
        requestType = htonl(requestType);
        if(write(sd, &requestType, sizeof(uint32_t)) < 0) {
            QMessageBox::warning(this, "WARNING", "Eroare la transmiterea cererii de creare cont la server!");
            exit(0);
        }
        User user;
        user.setUsername(ui->lineEdit_3->text().toStdString());
        user.setPassword(ui->lineEdit_4->text().toStdString());
        ui->lineEdit_3->setText("");
        ui->lineEdit_4->setText("");
        string serializedUser = SerializerDeserializer::serializeUser(user);
        Reader::sendMessageToServer(sd, this, serializedUser);
        string response = Reader::receiveMessageFromServer(sd, this);

        switch(response[0]) {
            case USER_EXISTS:
                        QMessageBox::warning(this, "WARNING", "Există deja un utilizator cu acest nume!");
                        return;
            case USER_NO_EXISTS :
                        QMessageBox::information(this, "SUCCESS", "Crearea contului a avut loc cu succes!");
        }

    }
}
