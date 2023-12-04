#include "formactivecalls.h"
#include "ui_formactivecalls.h"

FormActiveCalls::FormActiveCalls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormActiveCalls)
{
    ui->setupUi(this);
}

FormActiveCalls::~FormActiveCalls()
{
    delete ui;
}
