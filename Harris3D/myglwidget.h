// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include<set>
#include <vector>

// WE are going to create our OpenglComponent, by first inheriting from a QGLWidget (thanks to qt!)
// http://doc.qt.io/archives/qt-4.8/qglwidget.html
// then we add a couple of very useful functions
struct vertices
{
    float p1;
    float p2;
    float p3;
    std::set<int> adjacentpts;
};

struct faces
{
    int v1;
    int v2;
    int v3;

};

class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    vertices* vertex;
    faces* face;
    int* intPoints;
    int *canPoints;
    int tot_intPoints;
    int tot_canPoints;
    int tot_faces;
    int tot_vert;
    std:: string off;
    std:: string ini;
    std::string ini1;
    void calc();
    void setoff(std::string a){this->off = a;}
    void setini(std::string a){this->ini = a; this->ini1 =a;}
    void load_file_new (std::string filename, std::string file1);
    void NewDraw();
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:

public slots:

protected:
    // opengl functions

    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    //functions to handle widget stretching
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    //overloads to handle mouse events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZoom(int zoom);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zoomChanged(int zoom);

private:
    void draw(); // openGL code to draw a pyramid in this example

    int xRot;
    int yRot;
    int zRot;
    float zoomval ;

    QPoint lastPos; // last cursor location
};

#endif // MYGLWIDGET_H

