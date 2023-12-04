#include "mywork.h"
#include "QThread"
#include "QDebug"
#include "my.h"
#include "mycpp.h"
#include "myc.h"
MyWork::MyWork(QObject *parent)
    : QObject{parent}
{

}

void MyWork::nworking(int j)
{
    char usename[80];
    strcpy(usename,"1229");
    qt_make_new_call(usename);

    qDebug()<<"new working finished";
}



void MyWork::working(int i)
{
    int n=0;
    while(n<10000){
        n++;
        QString str=QString::number(n);
        emit(updateGUI(str));
        QThread::usleep(1);
    }
    qDebug()<<"working finished";
}
