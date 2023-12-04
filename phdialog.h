#ifndef PHDIALOG_H
#define PHDIALOG_H

#include <QDialog>
#include <QMetaEnum>
#include <QKeyEvent>
#include "mywork.h"
#include "QThread"
#include "mycpp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class phDialog; }
QT_END_NAMESPACE

//call states.
#define IDLE  0
#define DIALING 1
#define RINGING 2
#define TALKING 3
#define FAILED  4
#define CALLING 5
#define TODIAL 6

class phDialog : public QDialog
{
    Q_OBJECT

    int m_callState = 0;
    QList <QString> m_accountList;
    int curAccountID=-1;
    int unReviewedUnrecvedCalls = 0;
    bool m_DNDState=false;
    bool RJ45State=true;
public:
    phDialog(QWidget *parent = nullptr);
    ~phDialog();
    void IncomingCall(int Tabindex, QString RemoteAccount);

    void RegState(int status);
    void CallState();
    void ForwardCall();

    //keyboard event handler.
    void keyPressEvent(QKeyEvent *e);
    void showAccounts();
    void UpdateUI();//update UI according to the call state.
    void AddAccount(QString acc);//add a new account.
    void RegAccount(QString acc);//register an account.
    void OnTimer();
private slots:
    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_comboBox_editTextChanged(const QString &arg1);

    void on_pushButton_1_clicked();

    void on_comboBox_2_editTextChanged(const QString &arg1);

private:
    Ui::phDialog *ui;

    QThread* sub=nullptr;//a sub working thread
    MyWork* work = nullptr;//
    class1 *class1obj=nullptr;
    bool Xfer=false;


};
#endif // PHDIALOG_H
