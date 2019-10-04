#ifndef MYWIDGETS_H
#define MYWIDGETS_H

#include <QWidget>

namespace Ui {
class MyWidgets;
}

class MyWidgets : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidgets(QWidget *parent = nullptr);
    ~MyWidgets();

private:
    Ui::MyWidgets *ui;
};

#endif // MYWIDGETS_H
