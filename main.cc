#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"

#define sgn(a) ((a) > 0) ? (1.0) : (-1.0)

using namespace std;

double t=0;
double maxtime=10;
double dt=0.0001;
double outDt=0.05;
double L=30;
double G=-10;

size_t NL=5;
size_t NC=5;
size_t N=NL*NC;

class CWall{
	public:
	CWall(double _x1, double _y1, double _x2, double _y2){
		x1=_x1;
		x2=_x2;
		y1=_y1;
		y2=_y2;
		}

	void interact(CParticle &p){
		double wl=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

		double tx=(x2-x1)/wl,ty=(y2-y1)/wl;
		double nx=ty,ny=-tx;
		double qx=(p.x-x1),qy=(p.y-y1);
		int sg = 1;

		double l=(qx*tx + qy*ty);
		double d;

		if(l<0){
			d=sqrt(qx*qx + qy*qy);
			if( d > p.r)return;
			nx=qx/d; ny=qy/d;
		}else{

			if(l>wl){
				qx=(p.x-x2); qy=(p.y-y2);
				d=sqrt(qx*qx + qy*qy);
				if( d > p.r)return;
				nx=qx/d; ny=qy/d;
			}else{
				d=fabs(qx*nx + qy*ny);
				if( d > p.r)return;
				sg=sgn(qx*nx + qy*ny);
			};
		};

		double ovl=fabs(d - p.r);	//double vn=p.vy;
   		double fn=sg*p.kn*pow(ovl,1.5);
		p.fx+=fn*nx; p.fy+=fn*ny;

		}

	double x1, x2;
	double y1, y2;
	};

CWall wall1(0,0,L,0);
CWall wall2(0,0,0,L);
CWall wall3(L,0,L,L);
CWall wall4(0,L,L,L);

double cal_energy(CParticle *p){

	double e=0;
	for(int i=0; i<N; i++){
		e+=p[i].energy();
		e-=p[i].m*G*p[i].y;
		}

	return e;
	}

void cal_forces(CParticle p[]){

	
	for(int i=0; i<N; i++){
		p[i].fx=0;
		p[i].fy=G*p[i].m;

		wall1.interact(p[i]);
		wall2.interact(p[i]);
		wall3.interact(p[i]);
		wall4.interact(p[i]);

		for(int j=0; j<i; j++){
		p[i].interact(p[j]);

		}
	}
	}

void output(CParticle *p){
        static int count=0, outN=0,outPutN=outDt/dt;
        static ofstream out;

        if(count%outPutN==0 ){
                        stringstream outstream;
                        outstream<<"out"<<setw(5)<<setfill('0')<<outN;
			cerr<< outN<<endl;
                        out.open(outstream.str().c_str());
                        //walls.print(out);
                        //gout=&out;
		out<<"100000 0 0 0 0 0 0"<<endl;
		for(int i=0; i<N; i++){
			p[i].print(out);
                        }
			out.close();
                        count=0;
                        outN++;
			cout<< t <<"\t"<< cal_energy(p) <<endl;
                       }
		count++;

	}

int main(){



CParticle p[N];

//INIT
double v0=10;
 for(size_t i=0;i<NL;i++){
  for(size_t j=0;j<NC;j++){
   double r=1;
   p[i*NC+j].r=r;
   p[i*NC+j].set_pos(2.0*r+j*(L-2.5*r)/NC +(i%2)*0.5*L/NC, 2.0*r + i*L/NC) ;
   double theta=(rand()%100001)*M_PI*2.0e-5;
   p[i*NC+j].set_vel(v0*cos(theta), v0*sin(theta));
  }; 
 };


while (t<maxtime){

	for(int i=0; i<N; i++){
		p[i].predict(dt);
		}
	
	cal_forces(p);

	for(int i=0; i<N; i++){
		p[i].update_temp_accel();
	}

	for(int i=0; i<N; i++){
		p[i].correct();
		}

	for(int i=0; i<N; i++){
		p[i].update_accel();
	}

	t+=dt;
	output(p);
	}

return 0;
}
