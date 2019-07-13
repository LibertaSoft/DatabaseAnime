#ifndef LOOKPROGRESSBAR_H
#define LOOKPROGRESSBAR_H

#include <QWidget>
#include <QFrame>
#include <QString>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QCommonStyle>
#include <QStyleOption>

#include <QToolButton>
#include <QProgressBar>
#include <QHBoxLayout>

class LookProgressBar : public QWidget{
    Q_OBJECT
private:
    QToolButton  *_btnSub;
    QToolButton  *_btnAdd;
    QProgressBar *_lookProgressBar;
    QHBoxLayout  *_hLay;

    bool revertWhenOverflow;
    QString _targetFieldDB;

    void wheelEvent(QWheelEvent *e);
    void initCreate();  // First!
    void initConnect(); // Second!
public:
    explicit LookProgressBar(QWidget *parent = nullptr);
    explicit LookProgressBar(int minimum, int value, int maximum, QWidget *parent = nullptr);
    explicit LookProgressBar(int minimum, int value, int maximum, QString format, QString targetField, QWidget *parent = nullptr);

    void setValue(int);
    void setMaximum(int);
    void setMinimum(int);
    void setFormat(QString);
    void setRevertWhenOverflow(bool);
    void setTargetFieldDB(QString);

    int getValue()const;
    int getMaximum()const;
    int getMinimum()const;
    QString getFormat()const;
    QString getTargetField()const;

signals:
    void progressChanged(int, QString);
    void progressOverflow();
};

#endif // LOOKPROGRESSBAR_H
