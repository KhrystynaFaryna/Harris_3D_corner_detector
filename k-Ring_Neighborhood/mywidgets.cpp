#include "mywidgets.h"
#include "ui_mywidgets.h"

MyWidgets::MyWidgets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidgets)
{
    ui->setupUi(this);
}

MyWidgets::~MyWidgets()
{
    delete ui;
}
