// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H
#include <set>
#include <QGLWidget>

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
    int tot_intPoints;
    int tot_faces;
    int tot_vert;

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

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    void draw(); // openGL code to draw a pyramid in this example

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos; // last cursor location
};

#endif // MYGLWIDGET_H

