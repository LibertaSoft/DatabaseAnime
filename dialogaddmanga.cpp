#include "dialogaddmanga.h"
#include "ui_dialogaddmanga.h"

DialogAddManga::DialogAddManga(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddManga)
{
    ui->setupUi(this);
}

DialogAddManga::~DialogAddManga()
{
    delete ui;
}
