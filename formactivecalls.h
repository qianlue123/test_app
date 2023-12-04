#ifndef FORMACTIVECALLS_H
#define FORMACTIVECALLS_H

#include <QWidget>

namespace Ui {
class FormActiveCalls;
}

class FormActiveCalls : public QWidget
{
    Q_OBJECT

public:
    explicit FormActiveCalls(QWidget *parent = nullptr);
    ~FormActiveCalls();

private:
    Ui::FormActiveCalls *ui;
};

#endif // FORMACTIVECALLS_H
