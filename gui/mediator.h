#ifndef MEDIATOR_H
#define MEDIATOR_H 
#include<QObject>

class Mediator : public QObject
	{
	Q_OBJECT
	public:
	Mediator(mdGuiApp *gui){};
	public slots:
		void start();
 	private:
	};
#endif /* MEDIATOR_H */
