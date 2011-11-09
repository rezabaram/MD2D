
#include <QtGui> 
#include <QGraphicsEllipseItem>
#include <QGraphicsScene> 
#include <QGraphicsTextItem> 
#include <iostream> 
#include"gui.h"

mdGuiApp::mdGuiApp(QMainWindow *parent ){
	setupUi(this);

	QGraphicsScene *pscene=new QGraphicsScene;
	QGraphicsScene &scene=*pscene;
	scene.setSceneRect(-300, -300, 600, 600);

	QGraphicsEllipseItem ellipse(300,100,20,20);
	QGraphicsLineItem *line= new QGraphicsLineItem(0,0,100,100);
	QGraphicsLineItem line2(0,0,10,10);
	QGraphicsTextItem text("Hello World!");
	line->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	line2.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	
	scene.addItem(&ellipse);
	scene.addItem(line);
	scene.addItem(&line2);
	scene.addItem(&text);
	this->graphicsView->setScene(&scene);
	this->graphicsView->setInteractive(true);

	this->graphicsView->show();

	connect( buttonQuit, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( buttonStart, SIGNAL( clicked() ), this, SLOT( close() ) );
}
