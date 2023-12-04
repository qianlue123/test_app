// cpp.cpp

#include <iostream>
#include "mycpp.h"
#include "myc.h"
#include <QObject>
#include "QString"
#include "qt_pjsip_interface.h"
using namespace std; // 打开标准库名字空间
class1::class1()
{};
class1::~class1()
    {};

int class1::CallState(){
    emit(callState());
    return 0;
}
int class1::RegState(int status){
    emit(regState(status));
    return 0;
}
int class1::ForwardCall(){
    emit(forwardCall());
    return 0;
}


    int class1::IncomingCall(int i, char* str)
        {
        I++;

        emit(updateGUI(i, QString::fromUtf8(str)));
        cout<<"updateGUI committed";
        return 1;
        }

// 按 C 调用方式编译下面函数
extern "C"
int IncomingCall_C(struct1 * p, int index, char* str)
{
class1 * pClass1 = (class1 *)p;
cout << "c++: " << pClass1->IncomingCall(index,str) << endl;
return pClass1->IncomingCall(index,str);
}

extern "C"

int CallState_C(struct1 * p)
{
class1 * pClass1 = (class1 *)p;
cout << "c++: " << pClass1->CallState() << endl;
return pClass1->CallState();
}

extern "C"
int RegState_C(struct1 * p, int state)
{
class1 * pClass1 = (class1 *)p;
cout << "c++: " << pClass1->RegState(state) << endl;
return pClass1->RegState(state);
}
extern "C"
int ForwardCall_C(struct1 * p)
{
class1 * pClass1 = (class1 *)p;
cout << "c++: " << pClass1->ForwardCall() << endl;
return pClass1->ForwardCall();
}

// end file
