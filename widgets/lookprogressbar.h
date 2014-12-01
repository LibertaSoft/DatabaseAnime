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

class LookProgressBar2 : public QWidget{
    Q_OBJECT
private:
    QToolButton  *_btnSub;
    QToolButton  *_btnAdd;
    QProgressBar *_lookProgressBar;
    QHBoxLayout  *_hLay;

    bool revertWhenOverflow;
    QString _targetFieldDB;
    QString _targetOverflowFieldDB;

    void initCreate();  // One
    void initConnect(); // Two!
public:
    explicit LookProgressBar2(QWidget *parent = 0);
    explicit LookProgressBar2(int minimum, int value, int maximum, QWidget *parent = 0);
    explicit LookProgressBar2(int minimum, int value, int maximum, QString format, QString targetField, QWidget *parent = 0);

    void setValue(int);
    void setMaximum(int);
    void setMinimum(int);
    void setFormat(QString);
    void setTargetFieldDB(QString);
    void setTargetOverflowFieldDB(QString);

    int getValue()const;
    int getMaximum()const;
    int getMinimum()const;
    QString getFormat()const;
    QString getTargetField()const;
    QString getTargetOverflowFieldDB()const;

signals:
    void progressChanged(int, QString);
    void progressChanged(int); // Technical
    void progressOverflow();

};

class LookProgressBar : public QFrame
{
    Q_OBJECT
private:
    int _value;
    int _maxValue;
    int _minValue;
    bool _btnAddActive;
    bool _btnSubActive;
    QString _format;
    QString _targetFieldDB;
    QString _targetOverflowFieldDB;
public:
    explicit LookProgressBar(QWidget *parent = 0);
    virtual QSize sizeHint() const;

    void setValue(int);
    void setMaximum(int);
    void setMinimum(int);
    void setFormat(QString);
    void setTargetFieldDB(QString);
    void setTargetOverflowFieldDB(QString);
    bool setActiveBtnAdd(bool);
    bool setActiveBtnSub(bool);

    int getValue()const;
    int getMaximum()const;
    int getMinimum()const;
    QString getFormat()const;
    QString getTargetField()const;
    QString getTargetOverflowFieldDB()const;

    bool isActiveBtnAdd()const;
    bool isActiveBtnSub()const;    

signals:
    void progressChanged(int);
    void progressChanged(int, int, QString);
    void progressChanged(int, int, QString, QString);
public slots:
    void setProgress(int);
    void progressInc();
    void progressDec();

protected:
    virtual void paintEvent(QPaintEvent *pe);
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void leaveEvent(QEvent*);


};

#endif // LOOKPROGRESSBAR_H
