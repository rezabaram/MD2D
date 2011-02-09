#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"

using namespace std;

double t=0;
double maxtime=10;
double dt=0.000001;
double outDt=0.05;
double L=30;
double G=-10;

size_t NL=1;
size_t NC=1;
size_t N=NL*NC;

class CWall{
	public:
	CWall(double _x1=0, double _y1=0, double _x2=10, double _y2=0){
		x1=_x1;
		x2=_x2;
		y1=_y1;
		y2=_y2;
		}

	void interact(CParticle &p){

		double d=p.y-y1;
		if( d > p.r)return;
		double ovl= fabs(d - p.r);
		double vn=p.vy;

   		double fn=p.kn*pow(ovl,1.5);//-cn*vn; 

		p.fy+=fn;
		}

	double x1, x2;
	double y1, y2;
	};

CWall wall;

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
		//cerr<<p[i].m<<endl;
		}

	for(int i=0; i<N; i++){
		wall.interact(p[i]);
		}

	for(int i=0; i<N; i++){
		for(int j=i+1; j<N; j++){
		//cerr<< i <<"\t"<< j<<endl;
		p[i].interact(p[j]);
		}
	
	for(int i=0; i<N; i++){
		p[i].update_accel();
		//cerr<<p[i].m<<endl;
		}
	}
	}

void output(CParticle *p){
        static int count=0, outN=0,outPutN=outDt/dt;
        static ofstream out;
        //static double Energy=0.0, rEnergy=0, pEnergy=0, kEnergy=0;



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
                        //Energy=rEnergy+kEnergy+pEnergy;
                        //outEnergy<<setprecision(14)<<t<<"  "<<Energy<<"  "<<kEnergy<<"  "<<pEnergy<<"  "<<rEnergy <<endl;
                        //rEnergy=0; pEnergy=0; kEnergy=0; Energy=0;
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
   double theta=(rand()%100001)*6.28318e-5;
   p[i*NC+j].set_vel(v0*cos(theta), v0*sin(theta));
   p[i*NC+j].q=0;  
   p[i*NC+j].w=0;
  }; 
 };


while (t<maxtime){

	for(int i=0; i<N; i++){
		p[i].predict(dt);
		}
	
	cal_forces(p);

	for(int i=0; i<N; i++){
		p[i].correct();
		}
	
	cout<< t <<"\t"<< cal_energy(p) <<endl;
	t+=dt;

	output(p);
	}


 
return 0;
}
