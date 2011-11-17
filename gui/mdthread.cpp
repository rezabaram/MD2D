#include <QGraphicsEllipseItem>
#include<QGraphicsScene>
#include"mdthread.h"
#include"../mdsrc/vec.h"
#include"../mdsrc/particle.h"
#include"../mdsrc/mdsys.h"


extern CParticle *p;
extern int N;

QGraphicsEllipseItem **ellipses;
void MDThread::run(){
	Initialize();
	
	ellipses=new QGraphicsEllipseItem * [N];
	
	for(int i=0; i<N; i++){
		vec2d x=p[i].get_x();
		double w=2*p[i].get_r();
		ellipses[i]= new QGraphicsEllipseItem(600*x(0),600*x(1),600*w,600*w);
		scene->addItem(ellipses[i]);
		}
	
	Run();
	
};

void MDThread::add_particles(){
	for(int i=0; i<N; i++){
		vec2d x=p[i].get_x();
		double w=2*p[i].get_r();
		ellipses[i]->setRect(600*x(0),600*x(1),600*w,600*w);
		}
		scene->advance();
}

