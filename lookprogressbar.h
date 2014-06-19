#ifndef LOOKPROGRESSBAR_H
#define LOOKPROGRESSBAR_H

#include <QWidget>
#include <QFrame>
#include <QString>
#include <QPaintEvent>
#include <QMouseEvent>

class LookProgressBar : public QFrame
{
    Q_OBJECT
private:
    int _value;
    int _maxValue;
    int _minValue;
    QString _format;
    QString _type;
public:
    explicit LookProgressBar(QWidget *parent = 0);
    virtual QSize sizeHint() const;

    void setValue(int);
    void setMaximum(int);
    void setMinimum(int);
    void setFormat(QString str);
    void setType(QString);
signals:
    void progressChanged(int);
    void progressChanged(int, QString);
public slots:
    void setProgress(int);
    void progressInc();
    void progressDec();
protected:
    virtual void paintEvent(QPaintEvent *pe);
    virtual void mousePressEvent(QMouseEvent *pe);

};

#endif // LOOKPROGRESSBAR_H
