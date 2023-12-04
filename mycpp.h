#ifndef MYCPP_H
#define MYCPP_H
#include <QObject>
#include "QString"
#ifndef __cpp_h__
#define __cpp_h__
class class1 : public QObject{
     Q_OBJECT

public:
    class1();
    ~class1();
    int I;
    int J;
    int IncomingCall(int, char*);
    int CallState();
    int RegState(int);
    int ForwardCall();
signals:
    void updateGUI(int/*TabIndex 0..6*/, QString/*Remote caller ID*/);
    void callState();
    void regState(int);
    void forwardCall();
};
#endif
// end file

#endif // MYCPP_H
