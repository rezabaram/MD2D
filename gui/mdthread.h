#ifndef MDTHREAD_H
#define MDTHREAD_H 
#include<QThread>
#include<QGraphicsScene>

class MDThread : public QThread{

	Q_OBJECT
	public:
	MDThread(QGraphicsScene *_p):scene(_p){}
	virtual void run();
	public slots:
	void add_particles();
	private:
	QGraphicsScene *scene;

};


#endif /* MDTHREAD_H */
