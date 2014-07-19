#include "dialogadddorama.h"
#include "ui_dialogadddorama.h"

DialogAddDorama::DialogAddDorama(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddDorama)
{
    ui->setupUi(this);
}

DialogAddDorama::~DialogAddDorama()
{
    delete ui;
}
