#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>
#include<Socket.h>
#include<Book.h>

namespace Ui {
class Search;
}

class Search : public QDialog, public Socket
{
    Q_OBJECT

public:
    explicit Search(int sd, string username = "", QWidget *parent = 0);
    Search(list<Book> books, string username = "", QWidget* parent = 0);
    ~Search();

    list<Book> books;
    string username;

public slots:

    void viewButton(Book book, list<Book> books);

private slots:
    void on_commandLinkButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
