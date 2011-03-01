#ifndef DEGREE_OF_FREEDOM_H
#define DEGREE_OF_FREEDOM_H 
template<typename T>
class DFreedom
	{
	public:
	DFreedom(T &_x, T &_v, T &_a):x(_x), v(_v), a(_a){
		a0=(T)0.0;
		atemp=(T)0.0;
		}

	void predict(double dt){ // (Beeman's algorithm)
		 x+=(v + (2.0*a/3.0 - a0/6.0)*dt)*dt; 
		tempDt=dt;
		}

	void update_accel(const T & _a){
		atemp=_a;
		}

	void correct(){
		double dt=tempDt;
		double dtt=6.*dt*dt;
		v+=(atemp/3+5*a/6-a0/6)*dt;
		a0=a;
		a=atemp;
		}
 	private:
	T &x, &v, &a;
	T a0, atemp;
	double tempDt;
	};
#endif /* DEGREE_OF_FREEDOM_H */
