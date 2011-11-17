#include <QApplication>
#include <QTimer>
#include "gui.h"
#include "mdthread.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    mdGuiApp *mdApp= new mdGuiApp;
    mdApp->show();

    MDThread mdthread(mdApp->pscene);
    mdthread.start();
    //mdthread.wait();
    QObject::connect( mdApp, SIGNAL( need_to_plot() ), &mdthread, SLOT( add_particles() ) );
    //QObject::connect( mdApp, SIGNAL( need_to_plot() ), &mdthread, SLOT( add_particles(mdGui->scene) ) );
     QTimer timer;
     QObject::connect(&timer, SIGNAL(timeout()), &mdthread, SLOT( add_particles() ) );
     timer.start(1000 / 33);
    return app.exec();
}
