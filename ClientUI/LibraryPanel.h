#ifndef LIBRARYPANEL_H
#define LIBRARYPANEL_H

#include <QMainWindow>
#include <Socket.h>

namespace Ui {
class LibraryPanel;
}

class LibraryPanel : public QMainWindow, public Socket
{
    Q_OBJECT

public:
    explicit LibraryPanel(QWidget *parent = 0);
    ~LibraryPanel();

private:
    Ui::LibraryPanel *ui;
};

#endif // LIBRARYPANEL_H
