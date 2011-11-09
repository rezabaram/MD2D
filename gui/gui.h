#ifndef GUI_H
#define GUI_H 
 
#include <QGraphicsEllipseItem>
#include <QGraphicsScene> 
#include <QGraphicsTextItem> 
#include "ui_gui.h"
 
 
class mdGuiApp : public QMainWindow, private Ui::mdGui
{
    Q_OBJECT
 
public:

    mdGuiApp(QMainWindow *parent = 0);
 
public slots:
    //void getPath();
    //void doSomething();
    //void clear();
    //void about();
};


#endif /* GUI_H */
