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
    explicit View(QWidget *parent = 0);
    ~View();
    Book book;

private:
    Ui::View *ui;
};

#endif // VIEW_H
