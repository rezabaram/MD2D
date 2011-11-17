#ifndef MEDIATOR_H
#define MEDIATOR_H 
#include<QObject>
#include"../mdsrc/particle.h"


class Mediator : public QObject
	{
	Q_OBJECT
	public:
	Mediator(QGraphicsScene *_scene):scene(_scene){};
	void draw_particles();
	public slots:
		void start();
 	private:
	QGraphicsScene *scene;
	//CParticle *p;
	};
extern CParticle *p;
extern int N;
void Mediator::draw_particles(){
	for(int i=0; i<N; i++){
		vec2d x=p[i].get_x();
		double w=2*p[i].get_r();

		QGraphicsEllipseItem *ellipse= new QGraphicsEllipseItem(30*x(0),30*x(0),30*w,30*w);
		scene->addItem(ellipse);
	}

	}
#endif /* MEDIATOR_H */
