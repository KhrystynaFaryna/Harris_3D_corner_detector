// main.cpp

#include <QApplication>
#include <QDesktopWidget>
#include <iostream>
#include <fstream>
#include "harrisresponse.h"

using namespace std;
#include "window.h"

int main(int argc, char *argv[])
{
    string Dir;
    Dir = "C:\\Apple.off";
    HarrisResponse test(Dir);
    test.getNeighborhood_prime();
    string Otp;
    Otp = "C:\\Apple.ini.txt";



    cout<<endl;

    //set<int>ring1 = test.getkRinghood(test.vertex[693],1);
    //set<int>ring2 = test.getkRinghood(test.vertex[693],2);
    set<int>ring3 = test.getkRinghood(test.vertex[693],12);
    //set<int> ring4 = test.getkRinghood(test.vertex[693],4);
    //set<int> ring5 = test.getkRinghood(test.vertex[693],5);

    ofstream output(Otp);
    //output.open("Apple.ini.txt");
    if(output)
    {
        cout<<"I Created a file at location: "<<Otp<<endl;
        set<int>::iterator it;
        for(it = ring3.begin();it!= ring3.end();it++)
        {
            int val = *it;
            output << val <<" ";
        }
    }else
    {
        cout<<"ERROR, CANNOT CREATE THE FILE!"<<endl;

    }
    output.close();
    /*cout<<"First Ring: "<<endl;
    test.display_ringhood(ring1);
    cout<<endl<<endl;
    cout<<"Second Ring: "<<endl;
    test.display_ringhood(ring2);
    cout<<endl<<endl;
    cout<<"Third Ring: "<<endl;
    test.display_ringhood(ring3);
    cout<<endl<<endl;
    cout<<"Fourth Ring: "<<endl;
    test.display_ringhood(ring4);
    cout<<endl<<endl;
    cout<<"Fifth Ring: "<<endl;
    test.display_ringhood(ring5);
    cout<<endl<<endl;*/
    cout<<endl;
    cout<<"Going to display."<<endl;
    QApplication app(argc, argv);
    Window window;

    // When you handle several widgets with stretch factors (potentially with layouts)
    // you have to provide a sizeHint function to your components.

    // see https://www.ics.com/designpatterns/book/sizepolicy.html

    window.resize(window.sizeHint());

    //compute areas to determine if the application should be drawn maximized or not
    int desktopArea = QApplication::desktop()->width() *
            QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

    window.setWindowTitle("OpenGL with Qt");

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

    return app.exec();
}
