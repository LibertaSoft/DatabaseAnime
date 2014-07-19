#include "dialogaddamv.h"
#include "ui_dialogaddamv.h"

DialogAddAmv::DialogAddAmv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddAmv)
{
    ui->setupUi(this);
}

DialogAddAmv::~DialogAddAmv()
{
    delete ui;
}
