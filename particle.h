#ifndef PARTICLE_H
#define PARTICLE_H 
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
#include"object.h"
using namespace std;


MutexType tempmutex;
class CParticle: public CObject<2>{

	public:
	class CContact{
		public:
/*
			double d2=dist2(p);
			if( d2 > (r+p.r)*(r+p.r))return;
			double d=sqrt(d2);
			double ovl= fabs(d - (r+p.r));

			vec2d n=(get_x()-p.get_x())/d;
			vec2d t(n(1), -n(0));
			vec2d vr = get_v()-p.get_v() + r*(get_w() + p.get_w())*t;
*/
		CContact():p1(NULL),p2(NULL),exist(false){}
		CContact(CParticle *_p1,CParticle *_p2):p1(_p1),p2(_p2){
			//calculate();
			}
		void calculate(){
			vec2d r21=p1->get_x()-p2->get_x();
			double d2=r21.abs2();
			if( d2 > (p1->r+p2->r)*(p1->r+p2->r)){
				 exist=false;
				return;
				}
			else exist=true;
			double d=sqrt(d2);
			ERROR(d==0,"Interacting particles either identical or have identical centers.");
			dx= fabs(d - (p1->r+p2->r));
			n=r21/d;
			t=vec2d(n(1), -n(0));
			x=(p1->get_x()+p2->get_x()-p1->get_r()*n+p2->get_r()*n)/2.0;
			}
		void add(CParticle *p){
			}
		void apply_forces(){
		TRY
			if(!exist)return;
			//boost::mutex::scoped_lock lock1(this->freeze_mutex);
			//boost::mutex::scoped_lock lock2(this->freeze_mutex);
			//
			
			vec2d vr=dv();
			double fn=p1->kn*pow(dx,1.5)-p1->kd*(vr*n)*sqrt(dx);
			double vr_t = vr*t;
			
		//	double ft=(vr_t<vt)?(-mu*fn*vr_t/vt):(-mu*fn);    // dynamic friction approximation
			double ft=(-p1->mu*fn*vr_t/10.);

			vec2d df= fn*n + ft*t;
			p1->add_f(df);
			p1->add_tq(ft*p1->r);
	      
			p2->add_f(-df);
			p2->add_tq(ft*p2->r);
		CATCH
			}
		void print(ofstream &outf)const{
			outf<<"lines"<<endl;
			outf<< p1->get_x() <<"\t"<< p2->get_x()<<endl;
			outf<<"circles"<<endl;
			outf<< x<<"  "<<0.006<<endl;
			outf<<"lines"<<endl;
			}
		vec2d dv(){
			return p1->get_v()-p2->get_v() + (p1->r*p1->get_w() + p2->r*p2->get_w())*t;
			}
		vec2d x, n, t; 
		bool exist;
		double dx;
		CParticle *p1, *p2;
		};
	
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
	double get_r()const{
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
		//
		CContact contact(this, &p);
		contact.calculate();
		if(!contact.exist)return;
		
		
		vec2d vr=contact.dv();
   		double fn=kn*pow(contact.dx,1.5)-kd*(vr*contact.n)*sqrt(contact.dx);
		double vr_t = vr*contact.t;
		
	//	double ft=(vr_t<vt)?(-mu*fn*vr_t/vt):(-mu*fn);    // dynamic friction approximation
		double ft=(-mu*fn*vr_t/10.);

		vec2d df= fn*contact.n + ft*contact.t;
		add_f(df);
		add_tq(ft*r);
      
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
