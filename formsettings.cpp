#include "formsettings.h"
#include "ui_formsettings.h"
#include "mngrquerys.h"

#include <QDebug>
#include <QErrorMessage>

#include <QAbstractButton>
#include <QSettings>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);

    QSettings settings;

    bool a1 = settings.value( "enableSection/Anime",   true ).toBool();
    bool a2 = settings.value( "enableSection/Manga",  false ).toBool();
    bool a3 = settings.value( "enableSection/AMV",    false ).toBool();
    bool a4 = settings.value( "enableSection/Dorama", false ).toBool();

    bool b1 = settings.value( "optionalField/anime/OrigTitle",   false ).toBool();
    bool b2 = settings.value( "optionalField/anime/Director",    false ).toBool();
    bool b3 = settings.value( "optionalField/anime/PostScoring", false ).toBool();


    ui->CheckBox_EnableAnime->setChecked( a1 );
    ui->CheckBox_EnableManga->setChecked( a2 );
    ui->CheckBox_EnableAMV->setChecked( a3 );
    ui->CheckBox_EnableDorama->setChecked( a4 );

    ui->CheckBox_OrigTitle->setChecked( b1 );
    ui->CheckBox_Director->setChecked( b2 );
    ui->CheckBox_PostScoring->setChecked( b3 );


    // load tags in db
    SQM_AnimeTags = new QSqlQueryModel;
    SQM_AnimeTags->setQuery( QString("SELECT tagName FROM animeTags") );
    ui->ListView_Tags->setModel( SQM_AnimeTags );
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );

    QString set_language = settings.value( "Application/l10n", tr("<System>") ).toString();

    ui->CB_Language->addItem( tr("<System>"), 0 );
    QDir dir( QApplication::applicationDirPath() );
    QStringList l10n = dir.entryList( QStringList("DatabaseAnime_*.qm") );
    for(int i = 0; i < l10n.count(); ++i){
        ui->CB_Language->addItem( l10n.at(i).right(5).left(2) );
        if( set_language == l10n.at(i).right(5).left(2) )
            ui->CB_Language->setCurrentIndex(i+1);
    }
    Sort::sort sort = static_cast<Sort::sort>( settings.value( "Sorting", Sort::asc ).toInt() );
    ui->CBox_Sort->setCurrentIndex( sort );
}

FormSettings::~FormSettings()
{
    delete ui;
}

void FormSettings::on_BtnBox_accepted()
{
    QSettings settings;

    settings.setValue( "configExist", true );
    settings.setValue( "enableSection/Anime",     ui->CheckBox_EnableAnime->isChecked() );
    settings.setValue( "enableSection/Manga",     ui->CheckBox_EnableManga->isChecked() );
    settings.setValue( "enableSection/AMV",       ui->CheckBox_EnableAMV->isChecked() );
    settings.setValue( "enableSection/Dorama",    ui->CheckBox_EnableDorama->isChecked() );

    settings.setValue( "optionalField/anime/OrigTitle",   ui->CheckBox_OrigTitle->isChecked() );
    settings.setValue( "optionalField/anime/Director",    ui->CheckBox_Director->isChecked() );
    settings.setValue( "optionalField/anime/PostScoring", ui->CheckBox_PostScoring->isChecked() );

    settings.setValue( "Application/l10n", ui->CB_Language->currentText() );
    settings.setValue( "Application/l10n_index", ui->CB_Language->currentIndex() );
    settings.setValue( "Sorting", ui->CBox_Sort->currentIndex() );

}

void FormSettings::on_BtnBox_resetDefaults(){
    QMessageBox* pmbx =
    new QMessageBox(QMessageBox::Information,
                    tr("Warning"),
                    tr("<b>To return</b> the list of genres to a default state?"),
                    QMessageBox::Yes | QMessageBox::No |
                    QMessageBox::Cancel
                    );
    int n = pmbx->exec();
    delete pmbx;
    bool restoreTags = false;
    if (n == QMessageBox::Yes) {
        restoreTags = true;
    }else if( n == QMessageBox::No ){
        restoreTags = false;
    }else if( n == QMessageBox::Cancel ){
        return;
    }

    ui->CheckBox_EnableAnime->setChecked( true );
    ui->CheckBox_EnableManga->setChecked( false );
    ui->CheckBox_EnableAMV->setChecked( false );
    ui->CheckBox_EnableDorama->setChecked( false );

    ui->CheckBox_OrigTitle->setChecked( false );
    ui->CheckBox_Director->setChecked( false );
    ui->CheckBox_PostScoring->setChecked( false );
    ui->ListView_Tags->clearSelection();
    if( restoreTags ){
        MngrQuerys::insert_defaultAnimeTags();
    }

    ui->CB_Language->setCurrentIndex(0);
    ui->CBox_Sort->setCurrentIndex(1);
}

void FormSettings::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case  QDialogButtonBox::ApplyRole:
            emit on_BtnBox_accepted();
            this->close();
            break;
        case QDialogButtonBox::ResetRole:
            emit on_BtnBox_resetDefaults();
            SQM_AnimeTags->setQuery( "SELECT tagName FROM animeTags" );
            break;
        default:
            this->close();
    }
//    std::cout << "[Pressed:]" << ui->BtnBox->buttonRole( button ) << std::endl;
//    std::cout << "[Need:]" << QDialogButtonBox::ApplyRole << std::endl;
}

void FormSettings::on_TButton_DeleteTag_clicked()
{
    QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
    QSqlQuery query;
    query.prepare("DELETE FROM animeTags WHERE tagName = :tagName");
    for(int i = 0;i < mlist.count();i++){
        query.bindValue(":tagName", SQM_AnimeTags->data(mlist.at(i)).toString() );
        query.exec();
    }
    SQM_AnimeTags->setQuery( "SELECT tagName FROM animeTags" );
}

void FormSettings::on_TButton_AddTag_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO AnimeTags(tagName) VALUES (:newTagName) ");
    query.bindValue( ":newTagName", ui->LineEdit_AddTag->text() );
    ui->LineEdit_AddTag->clear();
    if( !query.exec() ){
        qDebug() << "Cannot insert new tag into AnimeTags: " << query.lastError().text();
    }
    SQM_AnimeTags->setQuery( "SELECT tagName FROM animeTags" );
}
