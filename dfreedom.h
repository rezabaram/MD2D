#ifndef DEGREE_OF_FREEDOM_H
#define DEGREE_OF_FREEDOM_H 
template<typename T>
class DFreedom
	{
	public:
	DFreedom(T &_x, T &_v, T &_a):x(_x), v(_v), a(_a), v_max(10e+14){
		a0=(T)0.0;
		atemp=(T)0.0;
		}

	void set_max(const double &max){
		v_max=max;
		}
	void predict(double dt){ // (Beeman's algorithm)
		//double dtt=6.*dt*dt;
		 x+=(v + (2.0*a/3.0 - a0/6.0)*dt)*dt; 
		tempDt=dt;
		}

	void update_accel(const T & _a){
		atemp=_a;
		}

	void correct(){
		double dt=tempDt;
		v+=(atemp/3+5*a/6-a0/6)*dt;


		a0=a;
		a=atemp;
		}
 	private:
	T &x, &v, &a;
	T a0, atemp;
	double tempDt, v_max;
	};

#endif /* DEGREE_OF_FREEDOM_H */
