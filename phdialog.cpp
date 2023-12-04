#include "phdialog.h"
#include "ui_phdialog.h"
#include "QTimer"
#include "QTime"
#include "QMovie"
//#include "formactivecalls.h"
#include "mywork.h"
#include "mycpp.h"
//#include "myc.h"
#include "qt_pjsip_interface.h"
#include "QThread"
#include "QFile"

extern qt_ua_info qt_ua_data;
extern void *s;
QString accname;
phDialog::phDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::phDialog)
{
    ui->setupUi(this);
    setStyleSheet("background-color:rgb(155,155,255)");

    //register with one of the accounts.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &phDialog::OnTimer);
    timer->start(1000);
    OnTimer();//定时刷新

    sub=new QThread;//a sub working thread
    work = new MyWork;//
    class1obj= new class1;
    s=(void *)class1obj;
    work->moveToThread(sub);//
    sub->start();//start the thread, but not doing anything
    //click the button to start working function.
    //connect(ui->pushButton, &QPushButton::clicked,work, &MyWork::working);
    //click the button2 to start working function.
    //connect(ui->pushButton_2, &QPushButton::clicked,work, &MyWork::nworking);

    //when signal from MyWork object received, main window will update.
 //   connect(work, &MyWork::updateGUI,this,&phDialog::UpdateUI);
 //   connect(class1obj,&class1::updateGUI,this,&phDialog::UpdateUI);

    connect(class1obj,&class1::updateGUI,this,&phDialog::IncomingCall);
    connect(class1obj,&class1::callState,this,&phDialog::CallState);
    connect(class1obj,&class1::regState,this,&phDialog::RegState);
    connect(class1obj,&class1::forwardCall,this,&phDialog::ForwardCall);





    //animation
    QMovie *movie=new QMovie(":/animated-telephone-image-0031.gif");
    QSize s(ui->label_Ringing->width()/2,ui->label_Ringing->height()/2);
    movie->setScaledSize(s);
    movie->start();
    ui->label_Ringing->setMovie(movie);


    movie=new QMovie(":/animated-telephone-image-0130.gif");
    s=QSize(ui->label_CALLING->width(),ui->label_CALLING->height());
    movie->setScaledSize(s);
    movie->start();
    ui->label_CALLING->setMovie(movie);

    ///FormActiveCalls *actCalls = new FormActiveCalls();
    //ui->stackedWidget->insertWidget(0,actCalls);

    qt_sip_main();
    //add some accounts



    //query the UA state via check the qt_ua_data struct:
    //qt_ua_data.call_cnt = 0;//# of active calls
    //qt_ua_data.currentcall_info//current call.
    //qt_ua_data.accout_arry[10];  10 accounts.
    //qt_ua_data.call_infos[3]   3 active calls at most.
    //qt_ua_data.conference_state

    //Read account file for registration.
    QFile file("account.txt");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         qDebug("unable to read configuration file, quit.");
         return;
    }

     QByteArray line;
     line = file.readLine();
     accname=line;
     accname=accname.trimmed();
     line = file.readLine();
     QString registra=line;
     registra=registra.trimmed();
     line = file.readLine();
     QString realm=line;
     realm=realm.trimmed();
     line = file.readLine();
     QString password=line;
     password=password.trimmed();
     qt_add_account(accname.toLatin1().data(),registra.toLatin1().data(),realm.toLatin1().data(),password.toLatin1().data());


    file.close();

     //AddAccount("1228"); // qt_add_account(username);
     UpdateUI();//切换到初始页面，以后根据协议切换。

}
void phDialog::showAccounts()
{
    for(int i=0;i<10;i++){
       // if(qt_ua_data.accout_arry[i].has_registration>0)

    }
}

phDialog::~phDialog()
{
    delete ui;
}

void phDialog::keyPressEvent(QKeyEvent *e){
    m_callState=TODIAL;
    UpdateUI();
}

void phDialog::OnTimer()
{

    //时钟更新
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    //日期更新
    this->ui->lcdNumber->display(text);
    QDate date=QDate::currentDate();
    text = date.toString("ddd MMMM d yyyy");
    this->ui->label_date->setText(text);

}

void phDialog::AddAccount(QString acc)//add a new account.
{

    qt_add_account(acc.toLatin1().data(),(char*)"10.5.43.172",(char*)"*",(char*)"sip123");
    //更新界面，添加号码，状态。
    ui->pushButton_1->setText(acc);
    if(1){//show icons according to the registration result.
        ui->pushButton_1->setIcon(QPixmap(":/Statusbar/Reg.png"));
    }
    else{
       ui->pushButton_1->setIcon(QPixmap(":/Statusbar/Unreg.png"));
    }
    ui->pushButton_1->setEnabled(true);
    m_accountList.append(acc);
    curAccountID=m_accountList.count()-1;
}
void RegAccount(QString acc)//register an account.
{

}
void phDialog::IncomingCall(int tabindex, QString cid)
{
    qDebug("Incoming Call triggered");
    ui->Label_callerID->setText(cid);
    m_callState=RINGING;
    UpdateUI();
}

void phDialog::RegState(int status){
    qDebug("Registration state changed.");

    ui->pushButton_1->setText(accname);
    if(status==200){//show icons according to the registration result.
        ui->pushButton_1->setIcon(QPixmap(":/Statusbar/Reg.png"));
    }
    else{
       ui->pushButton_1->setIcon(QPixmap(":/Statusbar/Unreg.png"));
    }
    ui->pushButton_1->setEnabled(true);
    m_accountList.append(accname);
    curAccountID=m_accountList.count()-1;
    UpdateUI();

}
void phDialog::CallState(){
    //switch the tab page according to the call state.
    //if confirmed, jump to TALKING
    qDebug("CallState called. state=%d",qt_ua_data.currentcall_info.state);
    if(qt_ua_data.call_cnt==0){
        m_callState=IDLE;
        UpdateUI();
        return;
    }
    switch(qt_ua_data.currentcall_info.state){
    case 0://PJSIP_INV_STATE_NULL,           /**< Before INVITE is sent or received  */
    break;
    case 1: //PJSIP_INV_STATE_CALLING,        /**< After INVITE is sent               */
    break;
    case 2://PJSIP_INV_STATE_INCOMING,       /**< After INVITE is received.          */
    break;
    case 3://PJSIP_INV_STATE_EARLY,          /**< After response with To tag.        */
    break;
    case 4://PJSIP_INV_STATE_CONNECTING,     /**< After 2xx is sent/received.        */
    break;
    case 5://PJSIP_INV_STATE_CONFIRMED,      /**< After ACK is sent/received.        */
        m_callState=TALKING;
        UpdateUI();
    break;
    case 6://PJSIP_INV_STATE_DISCONNECTED,   /**< Session is terminated.             */
        m_callState=IDLE;
        UpdateUI();


    break;
    }

    qDebug("#######################################\nCallState changed:%s\n %s****************************************",qt_ua_data.currentcall_info.remote_info,qt_ua_data.currentcall_info.state_text);
}
void phDialog::ForwardCall(){

}


void phDialog::UpdateUI()//update UI according to the call state.
{
    const int pixheight=32;
    const int pixwidth=32;
    QPixmap pixmap;
    //title bar:  set current Account, unreceived calls, DND, Network
    switch(m_callState){
    case IDLE:
        if(curAccountID==-1)
            ui->labelAccount->setText("No account");
        else
            ui->labelAccount->setText(accname);

        if(1){//show icons according to the registration result.
            pixmap=QPixmap(":/Statusbar/Reg.png");
        }
        else{
            pixmap=QPixmap(":/Statusbar/Unreg.png");
        }
        ui->labelAccount_2->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        break;

    case TODIAL:
        ui->labelAccount->setText("To dial");
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("1229");
        ui->comboBox_2->addItem("1230");
        ui->comboBox_2->addItem("1231");
        ui->comboBox_2->setMinimumWidth(5);
        break;
    case DIALING:
        ui->labelAccount->setText("DIALING");
        pixmap=QPixmap(":/Statusbar/Handsfree.png");
        ui->labelAccount_2->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        //initialize the combobox with the call history.
        ui->comboBox->clear();
        ui->comboBox->addItem("1229");
        ui->comboBox->addItem("1230");
        ui->comboBox->addItem("1231");
        ui->comboBox->setMinimumWidth(5);




        break;

    case FAILED:
        ui->labelAccount->setText("Failed");
        break;
    case CALLING:
        ui->labelAccount->setText("CALLING");
        break;

    case TALKING:
        ui->labelAccount->setText("TALKING");
        pixmap=QPixmap(":/Statusbar/Handsfree.png");
        ui->labelAccount_2->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        break;
    case RINGING:
        ui->labelAccount->setText("RINGING");

        break;
    }
    //current account.  icon may change according to call state. text: current account or none.


    if(m_DNDState){
        //m_pDNDstatus->setText("请勿打扰");//另外时钟下有一个显示DND状态。
        ui->labelDND->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->labelDNDinIDLE->setText("Do not disturb");
    }
    else{
        ui->labelDND->clear();
        ui->labelDNDinIDLE->setText("");
    }



    //unreviewed unreceived calls
    if(unReviewedUnrecvedCalls>0){
          pixmap= QPixmap(":/Statusbar/UnrecvedFlag.png");
          ui->labelUnRecv->setText("");
          ui->labelUnRecv->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }

    //DND status.
    pixmap= QPixmap(":/Statusbar/DND.png");
    if(m_DNDState){
        //m_pDNDstatus->setText("请勿打扰");//另外时钟下有一个显示DND状态。
        ui->labelDND->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        ui->labelDNDinIDLE->setText("Do not disturb");
    }
    else{
        ui->labelDND->clear();
        ui->labelDNDinIDLE->setText("");
    }

    //network status.
    ui->labelNet->setText("");
    if(RJ45State){
        pixmap= QPixmap(":/Statusbar/NetOn.png");
        ui->labelNet->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    else{
        pixmap= QPixmap(":/Statusbar/Netoff.png");
        ui->labelNet->setPixmap(pixmap.scaled(pixheight,pixwidth,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }



    //bottom:  set the captions of the buttons:
    switch(m_callState){
    case IDLE:
        ui->pushButton_11->setText("History");
        ui->pushButton_12->setText("Directory");
        ui->pushButton_13->setText("DND");
        ui->pushButton_14->setText("Menu");
        break;
    case CALLING:
        ui->pushButton_11->setText("");
        ui->pushButton_12->setText("");
        ui->pushButton_13->setText("");
        ui->pushButton_14->setText("Cancel");
        break;
    case TALKING:
        ui->pushButton_11->setText("Transfer");
        ui->pushButton_12->setText("Hold");
        ui->pushButton_13->setText("Conference");
        ui->pushButton_14->setText("End call");
        break;
    case FAILED:
        ui->pushButton_11->setText("");
        ui->pushButton_12->setText("");
        ui->pushButton_13->setText("");
        ui->pushButton_14->setText("Cancel");
        break;
    case RINGING:
        ui->pushButton_11->setText("Answer");
        ui->pushButton_12->setText("Transfer");
        ui->pushButton_13->setText("Mute");
        ui->pushButton_14->setText("Reject");
        break;

    case TODIAL:
        ui->pushButton_11->setText("Diretory");
        ui->pushButton_12->setText("");
        ui->pushButton_13->setText("");
        ui->pushButton_14->setText("Cancel");
        break;

    case DIALING:
        ui->pushButton_11->setText("Send");
        if(Xfer)
            ui->pushButton_12->setText("B-Xfer");
        else
            ui->pushButton_12->setText("");
        ui->pushButton_13->setText("Delete");
        ui->pushButton_14->setText("Cancel");
        break;

    }
    ui->stackedWidget->setCurrentIndex(m_callState);

}

void phDialog::on_pushButton_11_clicked()
{
    QString s;
    switch(m_callState){
    case IDLE:
        //show call history.
        break;
    case CALLING:
        break;
    case TODIAL:
        //popup the directory.
        break;

    case DIALING:
        //send， i.e. to make a call with the input number.
        s=ui->comboBox->currentText();
        qt_make_new_call(s.toLatin1().data());

        break;
    case RINGING:
        //answer the incoming call.
        qt_answer_call();
        m_callState=TALKING;
        UpdateUI();
        break;
    case TALKING:
        //switch to transfer, dial ...
        qt_call_hold();
        m_callState=TODIAL;
        Xfer=true;
        UpdateUI();
        break;
    default:
        break;

    }
}


void phDialog::on_pushButton_12_clicked()
{
    QString s;
    switch(m_callState){
    case IDLE:
        //popup the directory.
        break;
    case TALKING:
        //hold the call.
        qt_call_hold();
        break;
    case RINGING:
        //transfer the incoming call.
        //qt_forwarding_call();
        break;
    case DIALING:
        s=ui->comboBox->currentText();
        if(Xfer){
            qt_call_transfer(true, s.toLatin1().data());
            m_callState=IDLE;
            UpdateUI();
            Xfer=false;
        }
        break;
    default:
        break;

    }
}


void phDialog::on_pushButton_13_clicked()
{
    switch(m_callState){
    case IDLE://DND
        m_DNDState=!m_DNDState;
        UpdateUI();
        break;
    case DIALING:
        //delete the input in box.
        break;
    case RINGING:
        //mute.
        break;
    case TALKING:
        //conference.
        break;
    }
}


void phDialog::on_pushButton_14_clicked()
{
    switch(m_callState){
    case IDLE:
        //show the menu dialog.
        break;

    case FAILED:
    case CALLING:

    case TODIAL:
    case DIALING:
        //cancel.
        qt_hangup_call(1);
        m_callState=IDLE;
        UpdateUI();
        break;
    case RINGING:
        //reject the call.
        //qt_reject_call();
        m_callState=IDLE;
        UpdateUI();
        break;
    case TALKING:
        //end the call.
        qt_hangup_call(0);
        m_callState=IDLE;
        UpdateUI();
        break;
    }

}


void phDialog::on_comboBox_editTextChanged(const QString &arg1)
{
    //copy the string into the other combobox?  there are 2 pages.
    //use has input part of the URI, change the captions of the buttons, and the state too.
    if(m_callState==TODIAL){
      //  m_callState=DIALING;
        //UpdateUI();
    }
}


void phDialog::on_pushButton_1_clicked()
{

}


void phDialog::on_comboBox_2_editTextChanged(const QString &arg1)
{
    //user has entered a text in the editbox, switch to dialing page.
    ui->comboBox->setCurrentText(arg1);
    m_callState=DIALING;
    UpdateUI();
}

