#ifndef MYWORK_H
#define MYWORK_H

#include <QObject>
#include "QString"
class MyWork : public QObject
{
    Q_OBJECT
public:
    explicit MyWork(QObject *parent = nullptr);
    void working(int);
    void nworking(int);
signals:
    void updateGUI(QString);
};

#endif // MYWORK_H
