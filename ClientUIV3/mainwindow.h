#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Socket.h>
#include<string>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Socket
{
    Q_OBJECT

public:
    explicit MainWindow(int sd = 0, bool connected = false, string username = "", QWidget *parent = 0);
    ~MainWindow();

    string username;

private slots:

    void on_pushButton_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
