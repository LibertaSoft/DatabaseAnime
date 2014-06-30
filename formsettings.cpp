#include "formsettings.h"
#include "ui_formsettings.h"

#include <QDebug>
#include <QErrorMessage>

#include <QAbstractButton>
#include <QSettings>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>

bool insertDefaultTags_AnimeTags()
{
    QSqlQuery query;

    query.prepare("DELETE FROM AnimeTags");
    if( !query.exec() ){
        qDebug() << "Cannot truncate table AnimeTags: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }

    query.prepare("INSERT INTO AnimeTags(tagName) VALUES "
                  "(:v01), (:v02), (:v03), (:v04), (:v05), "
                  "(:v06), (:v07), (:v08), (:v09), (:v10), "
                  "(:v11), (:v12), (:v13), (:v14), (:v15), "
                  "(:v16), (:v17), (:v18), (:v19), (:v20), "
                  "(:v21), (:v22), (:v23), (:v24), (:v25), "
                  "(:v26), (:v27), (:v28), (:v29), (:v30), "
                  "(:v31), (:v32), (:v33), (:v34), (:v35), "
                  "(:v36), (:v37), (:v38), (:v39), (:v40), "
                  "(:v41), (:v42), (:v43), (:v44)"
                  );
    query.bindValue(":v01", "Сёнен");
    query.bindValue(":v02", "Сёнен Ай");
    query.bindValue(":v03", "Сейнен");
    query.bindValue(":v04", "Сёдзе");
    query.bindValue(":v05", "Сёдзе Ай");
    query.bindValue(":v06", "Дзёсей");
    query.bindValue(":v07", "Комедия");
    query.bindValue(":v08", "Романтика");
    query.bindValue(":v09", "Школа");
    query.bindValue(":v10", "Безумие");
    query.bindValue(":v11", "Боевые исскуства");
    query.bindValue(":v12", "Вампиры");
    query.bindValue(":v13", "Военное");
    query.bindValue(":v14", "Гарем");
    query.bindValue(":v15", "Демоны");
    query.bindValue(":v16", "Детское");
    query.bindValue(":v17", "Драма");
    query.bindValue(":v18", "Игры");
    query.bindValue(":v19", "Исторический");
    query.bindValue(":v20", "Космос");
    query.bindValue(":v21", "Магия");
    query.bindValue(":v22", "Машины");
    query.bindValue(":v23", "Меха");
    query.bindValue(":v24", "Мистика");
    query.bindValue(":v25", "Музыка");
    query.bindValue(":v26", "Пародия");
    query.bindValue(":v27", "Повседневность");
    query.bindValue(":v28", "Полиция");
    query.bindValue(":v29", "Приключения");
    query.bindValue(":v30", "Психологическое");
    query.bindValue(":v31", "Самураи");
    query.bindValue(":v32", "Сверхъестественное");
    query.bindValue(":v33", "Фурри");
    query.bindValue(":v34", "Спорт");
    query.bindValue(":v35", "Супер сила");
    query.bindValue(":v36", "Ужасы");
    query.bindValue(":v37", "Фантастика");
    query.bindValue(":v38", "Фэнтези");
    query.bindValue(":v39", "Экшен");
    query.bindValue(":v40", "Этти");
    query.bindValue(":v41", "Триллер");
    query.bindValue(":v42", "Хентай");
    query.bindValue(":v43", "Яой");
    query.bindValue(":v44", "Юри");

    if( !query.exec() ){
        qDebug() << "Cannot insert data in table nimeTags: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}

FormSettings::FormSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormSettings)
{
    ui->setupUi(this);

    QSettings settings;

    bool a1 = settings.value( "enableElem/BtnSwitchSection/Anime",     true ).toBool();
    bool a2 = settings.value( "enableElem/BtnSwitchSection/Manga",    false ).toBool();
    bool a3 = settings.value( "enableElem/BtnSwitchSection/AMV",      false ).toBool();
    bool a4 = settings.value( "enableElem/BtnSwitchSection/Dorama",   false ).toBool();
    bool a5 = settings.value( "enableElem/BtnSwitchSection/Editable",  true ).toBool();
    bool a6 = settings.value( "enableElem/BtnSwitchSection/LookLater", true ).toBool();

    bool b1 = settings.value( "enableElem/FieldsForEdit/OrigTitle",   false ).toBool();
    bool b2 = settings.value( "enableElem/FieldsForEdit/Director",    false ).toBool();
    bool b3 = settings.value( "enableElem/FieldsForEdit/PostScoring", false ).toBool();

    ui->CheckBox_EnableAnime->setChecked( a1 );
    ui->CheckBox_EnableManga->setChecked( a2 );
    ui->CheckBox_EnableAMV->setChecked( a3 );
    ui->CheckBox_EnableDorama->setChecked( a4 );
    ui->CheckBox_EnableEditing->setChecked( a5 );
    ui->CheckBox_EnableLookLater->setChecked( a6 );

    ui->CheckBox_OrigTitle->setChecked( b1 );
    ui->CheckBox_Director->setChecked( b2 );
    ui->CheckBox_PostScoring->setChecked( b3 );

    // load tags in db
    TableModel_Tags = new QSqlTableModel;
    TableModel_Tags->setTable("AnimeTags");
    TableModel_Tags->select();

    ui->ListView_Tags->setModel( TableModel_Tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setModelColumn(1);
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );
}

FormSettings::~FormSettings()
{
    delete ui;
}

void FormSettings::on_BtnBox_accepted()
{
    QSettings settings;

    settings.setValue( "configExist", true );
    settings.setValue( "enableElem/BtnSwitchSection/Anime",     ui->CheckBox_EnableAnime->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Manga",     ui->CheckBox_EnableManga->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/AMV",       ui->CheckBox_EnableAMV->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Dorama",    ui->CheckBox_EnableDorama->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/Editable",  ui->CheckBox_EnableEditing->isChecked() );
    settings.setValue( "enableElem/BtnSwitchSection/LookLater", ui->CheckBox_EnableLookLater->isChecked() );

    settings.setValue( "enableElem/FieldsForEdit/OrigTitle",   ui->CheckBox_OrigTitle->isChecked() );
    settings.setValue( "enableElem/FieldsForEdit/Director",    ui->CheckBox_Director->isChecked() );
    settings.setValue( "enableElem/FieldsForEdit/PostScoring", ui->CheckBox_PostScoring->isChecked() );

    // #Bug, need save tags in to database

}

void FormSettings::on_BtnBox_resetDefaults(){
    QMessageBox* pmbx =
    new QMessageBox(QMessageBox::Information,
                    "Ganres list",
                    "<b>To return</b> the list of genres to a default state?",
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
    ui->CheckBox_EnableEditing->setChecked( true );
    ui->CheckBox_EnableLookLater->setChecked( true );

    ui->CheckBox_OrigTitle->setChecked( false );
    ui->CheckBox_Director->setChecked( false );
    ui->CheckBox_PostScoring->setChecked( false );
    ui->ListView_Tags->clearSelection();

    if( restoreTags ){
        insertDefaultTags_AnimeTags();
        TableModel_Tags->select();
    }
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
            break;
        default:
            this->close();
    }
//    std::cout << "[Pressed:]" << ui->BtnBox->buttonRole( button ) << std::endl;
//    std::cout << "[Need:]" << QDialogButtonBox::ApplyRole << std::endl;
}

void FormSettings::on_TButton_DeleteTag_clicked()
{
    // #Bug : Удаление не реализовано
//    ui->ListView_Tags->
//    TableModel_Tags->
    QMessageBox::information(this, "Удаление", "Удаление выбранных элементов, будет реализовано позже.");

//    QStringList list; //Список для имен
//    QList idx; //Список для идентификаторов
//    QModelIndexList mlist = ui->ListView_Tags->selectionModel()->selectedIndexes();
//    QSqlQueryModel *mod = (QSqlQueryModel*)TableModel_Tags;//Получаем указатель на модель данных
//    QSqlQuery query;
//    query.prepare("DELETE FROM AnimeTags WHERE id = :id");
//    for(int i = 0;i < mlist.count();i++){
//        query.bindValue(":id", TableModel_Tags->ro .value("id").toInt());
//        query.exec();
//    }


}
