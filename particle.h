#ifndef PARTICLE_H
#define PARTICLE_H 
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<math.h>
using namespace std;


class CParticle{

	public:
	
	CParticle(){
		init();
		}

	void init(){
		kn=1e+6;
//		cn=0;//1e+2;                                 // after we put
		x=0; y=0; r=1;
		vx=0; vy=0;
		ax=0; ay=0;
		ax0=0; ay0=0;
//		q=0;w=0;                                 // after we put
//		aq=0; aq=0;
//		aq0=0; aq0=0;                                 // after we put
		fx=0; fy=0;
		density=1;
		update_mass();
		}
	void update_mass(){
		m=density*M_PI*r*r;
		}

	CParticle(double _x, double _y, double _r){
		init();
		set_pos(_x,_y);
		r=_r;
		density=1;
		}
	
	void set_pos(double _x, double _y ){
		x=_x; 
		y=_y;
		}

	void set_vel(double _vx, double _vy ){
		vx=_vx; 
		vy=_vy;
		}


	double dist(const CParticle &p)const{
		return sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y));
		}
	double dist2(const CParticle &p)const{
		return (x-p.x)*(x-p.x)+(y-p.y)*(y-p.y);
		}

	bool collide(const CParticle &p)const{
		};

	void interact(CParticle &p){
		double d2=dist2(p);
		if( d2 > (r+p.r)*(r+p.r))return;
		double d=sqrt(d2);
		double ovl= fabs(d - (r+p.r));
//		double vn=sqrt((vx-p.vx)*(vx-p.vx)+(vy-p.vy)*(vy-p.vy));

   		double fn=kn*pow(ovl,1.5);//-cn*vn; 

		double nx=(x-p.x)/d;
		double ny=(y-p.y)/d;

		fx+=fn*nx;
		fy+=fn*ny;

		p.fx-=fn*nx;
		p.fy-=fn*ny;

		}

	double energy(){
		return 0.5*m*(vx*vx+vy*vy);
		}

	void predict(double dt){
	 //atualzação das posições (Beeman's algorithm)
	 x+=(vx + (2.0*ax/3.0 - ax0/6.0)*dt)*dt; 
	 y+=(vy + (2.0*ay/3.0 - ay0/6.0)*dt)*dt; 
//	 q+=(w + (2.0*aq/3.0 - aq0/6.0)*dt)*dt;
	 //if(q>2.0*M_PI) q-=2.0*M_PI; if(q<0) q+=2.0*M_PI;
	tempDt=dt;
	}

	void update_accel(){
//		axtemp=ax;
//		aytemp=ay;
		ax=fx/m;
		ay=fy/m;
		//aq=T/I;
		}

	void update_accel2(){
		axtemp=fx/m;
		aytemp=fy/m;
		}

	void update_accel3(){
		ax0=ax;
		ay0=ay;
                x=xtemp;
                y=ytemp;
		}


	void correct(){
		static double dtt;
		double dt=tempDt;
		dtt=6.*dt*dt;
                xtemp=x;
                ytemp=y;
		x+=(2*ax-2*axtemp+ax0)*dtt;
		y+=(2*ay-2*aytemp+ay0)*dtt;
		//vx+=(5*ax+2*axtemp-ax0)*dt/12.0;
		//vy+=(5*ay+2*aytemp-ay0)*dt/12.0;
		vx+=(axtemp/3+5*ax/6-ax0/6)*dt;
		vy+=(aytemp/3+5*ay/6-ay0/6)*dt;

//		q+=(5*aq+2*aq1-aq0)*dt/12.0;                                            // after we put this
//	 	if(q>2.0*M_PI) q-=2.0*M_PI; if(q<0) q+=2.0*M_PI;
//		ax0=axtemp;
//		ay0=aytemp;
		}

	void print(ostream &out=cout)const{
		out<< x <<"\t"<< y <<"\t"<< r <<endl;
		}

	double kn; // cn;
	double x, y, r;
	double xtemp, ytemp;
	double vx, vy;
	double ax, ay;
	double ax0, ay0, axtemp, aytemp;
//	double w, q, aq, aq0, aq1;
	double fx, fy;

	double m;
	private:
	double density;
	double tempDt;

	};

#endif /* PARTICLE_H */
