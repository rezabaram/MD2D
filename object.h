#ifndef OBJECT_H
#define OBJECT_H 
#include"vec.h"
#include"dfreedom.h"

//This is the class from which any moving 
//object is derived. (Putting together the 
//common part, not to repeat it for every object.)

class CObject
	{
	public:
	CObject():translation(x,v,a), rotation(q, w, aq){
		x=0; v=0; a=0;
		q=0; w=0; aq=0;
		f=0; tq=0;
		m=1; Im=1;
		v_max=1e+14;
		}
	
	void predict(double dt){ // (Beeman's algorithm)
		translation.predict(dt);
		rotation.predict(dt);
		if (q>2.0*M_PI) q-=2.0*M_PI; if (q<0) q+=2.0*M_PI;
		}


	void correct(){
		translation.update_accel(f/m);
		rotation.update_accel(tq/Im);
		translation.correct();
		rotation.correct();
		check_max();
		}
	virtual void check_max(){
		}

	vec2d x,v,a, f;
	vec2d xtemp;
	double q,w,aq, tq;
	double m, Im;
	protected:
	DFreedom<vec2d> translation;
	DFreedom<double> rotation;
	double v_max;
 	private:
	};
#endif /* OBJECT_H */
