#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}
