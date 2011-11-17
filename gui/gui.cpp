
#include <QtGui> 
#include <QGraphicsEllipseItem>
#include <QGraphicsScene> 
#include <QGraphicsTextItem> 
#include <iostream> 
#include"gui.h"

mdGuiApp::mdGuiApp(QMainWindow *parent ){
	setupUi(this);

	pscene=new QGraphicsScene;
	QGraphicsScene &scene=*pscene;
	scene.setSceneRect(0, -0, 600, 600);


	this->graphicsView->setScene(&scene);
	this->graphicsView->setInteractive(true);
	this->graphicsView->show();

	connect( buttonQuit, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( buttonStart, SIGNAL( clicked() ), this, SLOT( plot() ) );


}
