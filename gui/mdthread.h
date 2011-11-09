#ifndef MDTHREAD_H
#define MDTHREAD_H 
#include<QThread>

class MDThread : public QThread{

//	Q_OBJECT
	public:
	void run();
	private:

};
#endif /* MDTHREAD_H */
