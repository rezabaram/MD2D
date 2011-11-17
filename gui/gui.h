#ifndef GUI_H
#define GUI_H 
 
#include <QGraphicsScene> 
#include <QGraphicsTextItem> 
#include "ui_gui.h"
 
#include<iostream>
using namespace std;
 
class mdGuiApp : public QMainWindow, private Ui::mdGui
{
    Q_OBJECT
 
public:

    mdGuiApp(QMainWindow *parent = 0);
 
    QGraphicsScene *pscene;

public slots:
    void plot(){
	emit need_to_plot();
		};
    //void getPath();
    //void doSomething();
    //void clear();
    //void about();
signals:
	void need_to_plot();

};


#endif /* GUI_H */
