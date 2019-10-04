#ifndef STARTINGWINDOW_H
#define STARTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class startingwindow;
}

class startingwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit startingwindow(QWidget *parent = nullptr);
    ~startingwindow();

private slots:
    void on_start_button_clicked();

private:
    Ui::startingwindow *ui;
};

#endif // STARTINGWINDOW_H
