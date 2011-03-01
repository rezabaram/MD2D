#ifndef PARTICLE_H
#define PARTICLE_H 
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include"vec.h"
#include"degree_of_freedom.h"
using namespace std;


class CParticle{

	public:
	
	CParticle():translation(x,v,a), rotation(q,w,aq){
		init();
		}

	void init();
	void update_mass(){
		m=density*M_PI*r*r;
		Im=m*r*r/2.0;
		}

	void set_r(double _r){
		r=_r;
		update_mass();
		}
	double get_r(){
		return r;
		}
	void set_pos(const vec2d &_x){
		x=_x; 
		}

	void set_vel(const vec2d &_v){
		v=_v; 
		}


/*	double dist(const CParticle &p)const{
		return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
		}
*/
	double dist2(const CParticle &p)const{
		return (x-p.x).abs2();
		}
	void interact(CParticle &p){
	TRY
		double d2=dist2(p);
		if( d2 > (r+p.r)*(r+p.r))return;
		double d=sqrt(d2);
		double ovl= fabs(d - (r+p.r));

   		double fn=kn*pow(ovl,1.5);

		ERROR(d==0,"Interacting particles either identical or have identical centers.");
		vec2d n=(x-p.x)/d;
		vec2d t(n(1), -n(0));

		vec2d vr = v-p.v + r*(w + p.w)*t;

		double vr_t = vr*t;
		
	//	double ft=(vr_t<vt)?(-mu*fn*vr_t/vt):(-mu*fn);    // dynamic friction approximation
		double ft=(-mu*fn*vr_t/10.);

		vec2d df= fn*n + ft*t;
		f+=df;
		if(!rotation_fixed) tq+=ft*r;
      
		p.f-=df;
		if(!p.rotation_fixed) p.tq+=(ft*p.r);
	CATCH
		}
	void fix_rotation(double w0){
		w=w0;
		rotation_fixed=true;
		}

	double energy(){
		return 0.5*m*v.abs2();
		}

	void predict(double dt){ // (Beeman's algorithm)
		translation.predict(dt);
		rotation.predict(dt);
		if (q>2.0*M_PI) q-=2.0*M_PI; if (q<0) q+=2.0*M_PI;
		}

	void update_accel(){
		translation.update_accel(f/m);
		rotation.update_accel(tq/Im);
		}

	void correct(){
		translation.correct();
		rotation.correct();
		}

	void print(ostream &out=cout)const{
		out<< x <<"\t"<< r <<"\t"<<q<<"\t"<<w<<endl;
		}

	double kn,mu,vt;
	vec2d x, v, a;
	double vtr; //velocidade de transição

	double w, q, aq;
	vec2d f;
	double tq;
	double m, Im;
	bool rotation_fixed;
	private:

	double density;
	double r;
	double tempDt;
	vec2d xtemp;
	vec2d a0,  atemp;
	double aqtemp, aq0;
	DFreedom<vec2d> translation;//translational
	DFreedom<double> rotation;//rotational
	};

void CParticle::init() {
	rotation_fixed=false;
	kn=1e+6; mu=0.8;
	x=0; r=1;
	v=0;
	a=0;
	a0=0;
	q=0;w=0;
	vt=2.0;
	aq=0; aq=0;
	aq0=0; aq0=0;
	f=0; tq=0;
	density=1;
	update_mass();
	}

#endif /* PARTICLE_H */
