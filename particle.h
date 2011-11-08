#ifndef PARTICLE_H
#define PARTICLE_H 
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include"object.h"
using namespace std;

class CParticle;
class CContact
	{
	public:
	//CContact(const vec2d &_x,const vec2d &_f,const  vec2d &_v):dx(_x),f(_f),v(_v){}
	CContact(const vec2d &_x,double dis):dx(_x),dissipation(dis){}
	vec2d dx;
	//vec2d dx, f, v;
	double dissipation;
 	private:
	};

MutexType tempmutex;
class CParticle: public CObject<2>{

	public:
	
	CParticle(){
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


/*	double dist(const CParticle &p)const{
		return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
		}
*/
	double dist2(const CParticle &p)const{
		return (get_x()-p.get_x()).abs2();
		}
	void interact(CParticle &p){
	TRY
		//boost::mutex::scoped_lock lock1(this->freeze_mutex);
		//boost::mutex::scoped_lock lock2(this->freeze_mutex);
		double d2=dist2(p);
		if( d2 > (r+p.r)*(r+p.r))return;
		double d=sqrt(d2);
		double ovl= fabs(d - (r+p.r));

		vec2d n=(get_x()-p.get_x())/d;
		vec2d t(n(1), -n(0));
		vec2d vr = get_v()-p.get_v() + r*(get_w() + p.get_w())*t;

   		double fn=kn*pow(ovl,1.5)-kd*(vr*n)*sqrt(ovl);

		ERROR(d==0,"Interacting particles either identical or have identical centers.");

		double vr_t = vr*t;
		
	//	double ft=(vr_t<vt)?(-mu*fn*vr_t/vt):(-mu*fn);    // dynamic friction approximation
		double ft=(-mu*fn*vr_t/10.);

		vec2d df= fn*n + ft*t;
		add_f(df);
		add_tq(ft*r);
     		if(save_contacts)contacts.push_back(CContact(p.get_x()-x,vr_t*ft));
 
		p.add_f(-df);
		p.add_tq(ft*p.r);
	CATCH
		}
	void fix_rotation(double w0){
		set_w(w0);
		rotation_fixed=true;
		}

	double energy(){
		return 0.5*m*get_v().abs2();
		}


	void print(ostream &out=cout)const{
		out<< get_x() <<"\t"<< r <<"\t"<<get_q()<<"\t"<<get_w()<<endl;
		}

	double kn, kd,mu,vt;
	double vtr; //velocidade de transição

	vector <CContact> contacts;
	bool save_contacts;
	private:

	double density;
	double r;
	
	};

void CParticle::init() {
	rotation_fixed=false;
	kn=1e+5; kd=30; mu=0.8;
	r=1;
	vt=2.0;
	density=1;
	update_mass();
	}

#endif /* PARTICLE_H */
