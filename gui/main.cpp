#include <QApplication>
#include "gui.h"
#include "mdthread.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    mdGuiApp *mdApp= new mdGuiApp;
    mdApp->show();
    MDThread mdthread;
    mdthread.start();
    //mdthread.wait();
    return app.exec();
}
