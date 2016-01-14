#ifndef Dialog_H
#define Dialog_H
#pragma once
#include <QDialog>
#include<Socket.h>
#include<string>

using namespace std;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, public Socket
{
    Q_OBJECT

public:
    explicit Dialog(string username = "", QWidget *parent = 0);
    ~Dialog();

    string username;

private slots:
    void on_cauta_clicked();

    void on_pushButton_clicked();

    void on_recomanda_clicked();

private:
    Ui::Dialog *ui;
};
#endif // Dialog2_H
