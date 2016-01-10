#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include<Socket.h>
#include<Book.h>

namespace Ui {
class View;
}

class View : public QDialog, public Socket
{
    Q_OBJECT

public:
    explicit View(Book book, list<Book> books, string username = "", QWidget *parent = 0);
    ~View();

    Book book;
    list<Book> books;
    string username;

private slots:
    void on_toolButton_clicked();

    void on_pushButton_clicked();

    void on_commandLinkButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::View *ui;
};

#endif // VIEW_H
