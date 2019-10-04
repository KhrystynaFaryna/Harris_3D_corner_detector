// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<set>
#include<algorithm>
#include "myglwidget.h"
#include "harrisresponse.h"
#include "settingsform.h"

std::string intfile ="C:\\models_OFF\\model.ini.txt";
using std::endl;
using std::cout;
using std::cin;
using std::cerr;


MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
}

void MyGLWidget::load_file_new (std::string filename,std::string file1)
{
    std::string canfil = "C:\\models_OFF\\modelcan.ini.txt";
    std::ifstream OFF_file(filename);
    xRot = 0;
    yRot = 0;
    zRot = 0;
    zoomval =1;

    //number of faces & vertices
    float temp0,temp1,temp2,temp3;
    int mem_loc=0;

    int no_vert;
    int no_face;
    int no_edges;
    int no_intPoints=0;
    int no_canPoints =0;
    std:: string  ModelType;

    if(OFF_file>>ModelType>>no_vert>>no_face>>no_edges)

    {
        cout<<endl<<"The model type is: "<<ModelType<<endl;
        cout<<endl<<"The number of vertices is: "<<no_vert<<endl;
        cout<<endl<<"The number of faces is: "<<no_face<<endl;
        cout<<endl<<"The number of edges is: "<<no_edges<<endl;

    }
    vertex=new vertices[no_vert];
    face=new faces[no_face];

    this->tot_vert= no_vert;
    this->tot_faces=no_face;
    for (mem_loc=0; mem_loc<=no_vert-1; mem_loc++)
    {
        OFF_file>>temp1>>temp2>>temp3;
        vertex[mem_loc].p1=temp1;
        vertex[mem_loc].p2=temp2;
        vertex[mem_loc].p3=temp3;

    }

    for (mem_loc=0; mem_loc<=no_face-1; mem_loc++)
    {
        OFF_file>>temp0>>temp1>>temp2>>temp3;
        face[mem_loc].v1=temp1;
        face[mem_loc].v2=temp2;
        face[mem_loc].v3=temp3;
    }

    this->vertex=vertex;

    mem_loc=0;

    this->face=face;

    std::ifstream INT_file(file1);
    while(INT_file>>temp1)
    {
        no_intPoints=no_intPoints+1;
    }
    this->tot_intPoints=no_intPoints;

    int* intPoints=new int [no_intPoints];
    std::ifstream INT_file1(file1);
    for (int i=0; i<no_intPoints; i++)
    {
        INT_file1>>temp1;
        intPoints[i]=temp1;
    }
    this->intPoints=intPoints;

    for (int i=0; i<no_vert; i++)
    {
        for (int j=0; j<no_face; j++)
        {
            if ((face[j].v1 == i) || (face[j].v2 == i) || (face[j].v3 == i))
            {
                vertex[i].adjacentpts.insert(face[j].v1);
                vertex[i].adjacentpts.insert(face[j].v2);
                vertex[i].adjacentpts.insert(face[j].v3);
            }
        }
    }

    INT_file.close();
    INT_file1.close();
    OFF_file.close();


    std::ifstream CAN_file(canfil);
    while(CAN_file>>temp1)
    {
        no_canPoints=no_canPoints+1;
    }
    this->tot_canPoints=no_canPoints;

    int* canPoints=new int [no_canPoints];
    std::ifstream CAN_file1(canfil);
    for (int i=0; i<no_canPoints; i++)
    {
        CAN_file1>>temp1;
        canPoints[i]=temp1;
    }
    this->canPoints=canPoints;

    CAN_file1.close();
    CAN_file.close();

    draw();
}
//
MyGLWidget::~MyGLWidget()
{
}

//minimumSizeHint and sizeHint functions are needed when resizing the window
QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(1000, 1000);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(2000, 2000);
}

static void qNormalizeAngle(int &angle)
{
    // urgl, it kind of hurts, but it works ;)
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

//implementation of the 3 slots to catch rotation values from sliders and update the openglwidget
void MyGLWidget::setXRotation(int angle)
{
    //we assert the rotation value makes sense
    qNormalizeAngle(angle);

    if (angle != xRot) {//if rotation has changed
        xRot = angle; // update value
        emit xRotationChanged(angle); // emit a signal that carries the angular value
        updateGL(); // draw the scene
    }
}

// same goes for the Y and Z axis
void MyGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void MyGLWidget::setZoom(int zoom)
{

    if(zoom!=zoomval){

        zoomval = zoom;
        cout<<zoomval<<endl;
        updateGL();
    }
}

void MyGLWidget::initializeGL()
{
    //init background color
    qglClearColor(Qt::white);

    //enable depth test
    glEnable(GL_DEPTH_TEST);

    //enable face culling, see https://learnopengl.com/Advanced-OpenGL/Face-culling
    //  glEnable(GL_CULL_FACE);

    //enable smooth (linear color interpolation)
    //glShadeModel(GL_SMOOTH);





    float diffuse0[]={0.2, 0.2, 0.2, 1.0};
    float ambient0[]={0.8, 0.8, 0.8, 1.0};
    float specular0[]={1.0,1.0, 1.0, 1.0};
    //float light0_pos[]={0.0, 0.0, 1.0, 1.0};


    // glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

    glEnable(GL_LIGHTING);


    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shine[] = {20.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
    glEnable ( GL_COLOR_MATERIAL ) ;

    //we need at least 1 light to see something
    glEnable(GL_LIGHTING);
    //    //light0 is the default one
    glEnable(GL_LIGHT0);
    //we now set the light position
    glScalef(zoomval, zoomval, zoomval);

    GLfloat lightPosition[4] = { 0, 0, 15, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_NORMALIZE);
}

void MyGLWidget::NewDraw()
{
    paintGL();
    draw();
    updateGL();
}

void MyGLWidget::paintGL()
{
    //first, clear everything
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //reset any transform
    glLoadIdentity();

    //translate to see the object. Comment that ligne to see the changes
    glTranslatef(0.0, 0.0, 0.0);

    //apply rotations along x, y, then z axis
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    //glScalef(zoomval, zoomval, zoomval);

    draw();
}


//resizeGL is used when the GLwidget is resized
void MyGLWidget::resizeGL(int width, int height)
{

    int side = qMin(width, height);
    glViewport((width - side)/ 2, (height - side)/ 2, side, side);

    //glMatrixMode is used to activate the world in which some transforms will be applied
    //https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glMatrixMode.xml

    glMatrixMode(GL_PROJECTION); //basically, we are working in the rendered image, in pixels
    glLoadIdentity();



    //SET IT TO A VARIABLE AND ASSIGN TO A SLIDER
    //WE ARE HERE
    glScalef(1.0f,1.0f,1.0f);

    //we are done with the positionning and stretching of the rendered image,
    //we then reset the matrix mode to model view,
    //so that the rendered objet is rotated whenever we move the rotation cursors
    glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }

    lastPos = event->pos();
}

void MyGLWidget::draw()
{

    float nx=0,ny=0,nz=0,ux=0,uy=0,uz=0,vx=0,vy=0,vz=0;
    float norm;
    for(int i = 0; i < tot_faces; i++)
    {


        ux=(this->vertex[this->face[i].v3].p1)-(this->vertex[this->face[i].v1].p1);
        uy=(this->vertex[this->face[i].v3].p2)-(this->vertex[this->face[i].v1].p2);
        uz=(this->vertex[this->face[i].v3].p3)-(this->vertex[this->face[i].v1].p3);

        vx=(this->vertex[this->face[i].v2].p1)-(this->vertex[this->face[i].v1].p1);
        vy=(this->vertex[this->face[i].v2].p2)-(this->vertex[this->face[i].v1].p2);
        vz=(this->vertex[this->face[i].v2].p3)-(this->vertex[this->face[i].v1].p3);

        nx=uy*vz-uz*vy;
        ny=uz*vx-ux*vz;
        nz=ux*vy-uy*vx;

        norm=sqrt(nx*nx+ny*ny+nz*nz);

        nx=nx/norm;
        ny=ny/norm;
        nz=nz/norm;
        glColor3f(0.2f, 0.2f,0.2f);
        glBegin(GL_TRIANGLES);
        glNormal3f(nx,ny,nz);
        //vertex3f takes the 3 coordinates for one vertex(each line one vertex)
        glVertex3f((this->vertex[this->face[i].v1].p1) ,(this->vertex[this->face[i].v1].p2) ,(this->vertex[this->face[i].v1].p3) );
        glVertex3f((this->vertex[this->face[i].v2].p1) ,(this->vertex[this->face[i].v2].p2) ,(this->vertex[this->face[i].v2].p3) );
        glVertex3f((this->vertex[this->face[i].v3].p1) ,(this->vertex[this->face[i].v3].p2) ,(this->vertex[this->face[i].v3].p3) );
        glEnd();
    }

    // INTEREST POINTS
    glPointSize(10.0f);
    for(int i=0;i<tot_intPoints; i++)
    {
        glColor3f(1.0f,0.0f,0.0f);
        glBegin(GL_POINTS); //starts drawing of points
        glVertex3f(this->vertex[intPoints[i]].p1,this->vertex[intPoints[i]].p2 ,this->vertex[intPoints[i]].p3);//upper-right corner
        glEnd();//end drawing of points
    }


   // CANDIDATE POINTS
   glPointSize(10.0f);
    for(int i=0;i<tot_canPoints; i++)
    {
        glColor3f(0.0f,1.0f,1.0f);
        glBegin(GL_POINTS); //starts drawing of points
        glVertex3f(this->vertex[canPoints[i]].p1,this->vertex[canPoints[i]].p2 ,this->vertex[canPoints[i]].p3);//upper-right corner
        glEnd();//end drawing of points
    }





    // EDGES IN RING
    int EdgesinRing = 0;
    for(int i=0; i<tot_intPoints; i++)
    {
        int P = intPoints[i];                                                   //For each Interest/Ring Point

        std::set<int>N = vertex[P].adjacentpts;                        //Find its Neighbour Vertices

        for(std::set<int>::iterator iti = N.begin(); iti != N.end();iti++)
        {
            int nei=*iti;

            for(int k=0; k<tot_intPoints; k++)
            {
                if (nei==intPoints[k])                                          //Draw an Edge if Neighbour Vertice is in Ring
                {
                    glLineWidth(7.0);
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glBegin(GL_LINES); //starts drawing of edges
                    glVertex3f(this->vertex[intPoints[i]].p1,this->vertex[intPoints[i]].p2 ,this->vertex[intPoints[i]].p3);
                    glVertex3f(this->vertex[intPoints[k]].p1,this->vertex[intPoints[k]].p2 ,this->vertex[intPoints[k]].p3);
                    glEnd(); //end drawing of edges
                    EdgesinRing++;
                }
            }
        }




    }

}
