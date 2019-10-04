#include "settingsform.h"
#include "ui_settingsform.h"
# include "harrisresponse.h"
#include "myglwidget.h"
#include <QFileDialog>
#include <QDesktopWidget>
#include <QMessageBox>
#include <iostream>
#include <fstream>


using namespace std;
settingsform::settingsform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingsform)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings Form");
    ui->widget->hide();
    /*connect(ui->widget,SIGNAL(xRotationChanged(int)), ui->xRotSlider,  SLOT(setValue(int)));
    connect(ui->widget,SIGNAL(yRotationChanged(int)), ui->y_RotSlider, SLOT(setValue(int)));
    connect(ui->widget,SIGNAL(zRotationChanged(int)), ui->zRotSlider,  SLOT(setValue(int)));
    connect(ui->widget,SIGNAL(zoomChanged(int)),    ui->verticalSlider, SLOT(setValue(int)));
*/
}


settingsform::~settingsform()
{
    delete ui;
}

void settingsform::on_mesh_button_clicked()
{

     QString mesh_path = QFileDialog::getOpenFileName(this, tr("Load The OFF File"),"C://", "OFF File(*.off)");
    ui->mesh_lab->setText(mesh_path);
    QString tmp = ui->mesh_lab->text();
    //need to set.
    //settingforms::setmeshfile(tmp.toLocal8Bit().constData());
    mesh_file = tmp.toLocal8Bit().constData();


}

void settingsform::on_conf_par_button_clicked()
{
    QString val = ui->ring_text->toPlainText();
    for(int i=0; i<val.length();i++)
    {
        if (val.at(i).isDigit())
        {
            this->setring(val.toInt());

        }
    }
}

void settingsform::on_display_button_clicked()
{
    string tmp;
    cout<<"Creating th INI file..."<<endl;
    size_t loc = mesh_file.find_last_of(".");
    tmp = mesh_file.substr(0,loc);
    string ini_file = tmp+ ".ini.txt";
    cout<<"Ini file: "<<ini_file<<endl;
    cout << "OFF FILE: " << mesh_file <<endl;
    cout<<"Initializing the operator."<<endl;
    HarrisResponse harris(mesh_file);
    cout<<endl;
    cout<<"Everything is fine, calculating the first ringhood."<<endl;
    harris.getNeighborhood_prime();
    cout<<endl;
    cout<<"Calculating the "<<noring<<"TH ringhood"<<endl;

    cout<<"Calculating.."<<endl;

    cout<<"DONE."<<endl;
    cout<<"Display: "<<endl;
    tmp = "C:\\models_OFF\\modelcan.ini.txt";
    harris.computeHarrisResponse(noring);
    harris.findcandidates(ini_file, tmp);
    ui->widget->show();
    ui->widget->load_file_new(mesh_file, ini_file);
    ui->widget->NewDraw();

}


