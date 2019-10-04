#include "startingwindow.h"
#include "ui_startingwindow.h"
#include "settingsform.h"

startingwindow::startingwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startingwindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Harris 3D, A Robust implementation");
    ui->statusBar->showMessage("Pierpaolo Vendittelli, Khrystyna Faryna, Anindya Shaha");
}

startingwindow::~startingwindow()
{
    delete ui;
}

void startingwindow::on_start_button_clicked()
{
    this->hide();
    settingsform *form = new settingsform();
    form->show();

}
