#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include "myglwidget.h"
namespace Ui {
class settingsform;
}

class settingsform : public QWidget
{
    Q_OBJECT

public:
    explicit settingsform(QWidget *parent = nullptr);
    std::string getmesh(){return this->mesh_file;}
    int getring(){return this->noring;}
    void setmesh(std:: string a){this->mesh_file = a;}
    void setring(int a){this->noring = a;}
    ~settingsform();

private slots:
    void on_mesh_button_clicked();

    void on_conf_par_button_clicked();

    void on_display_button_clicked();


private:
    std:: string mesh_file;
    int noring;
    Ui::settingsform *ui;
};

#endif // SETTINGSFORM_H
