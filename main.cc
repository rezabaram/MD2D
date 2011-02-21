#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"
#include"wall.h"


using namespace std;

double t=0;
double maxtime=10;
double dt=0.0001;
double outDt=0.01;
double L=20;
double G=-10;

size_t NL=20;
size_t NC=20;
size_t N=NL*NC;


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
		p[i].tq=0;

		wall.interact(p[i]);

		for(int j=0; j<i; j++){
		p[i].interact(p[j]);
		if(p[j].has_shadow){
			p[j].shift_to_shadow();
			p[i].interact(p[j]);
			p[j].shift_back();
			}

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

wall.add_segment(0,0,L,0);
wall.add_segment(0,0,0,L);
wall.add_segment(L,0,L,L);
wall.add_segment(0,L,L,L);


CParticle p[N];

//INIT
double v0=10;
 for(size_t i=0;i<NL;i++){
  for(size_t j=0;j<NC;j++){
   double r=L/(double)(NC+1)/2.9;
   p[i*NC+j].r=r*(1+.3*(1-drand48()));
   p[i*NC+j].set_pos(2.0*r+j*(L-2.5*r)/NC +(i%2)*0.5*L/NC, 2.0*r + i*L/NC) ;
   double theta=(rand()%100001)*M_PI*2.0e-5;
   p[i*NC+j].set_vel(v0*cos(theta), v0*sin(theta));
  }; 
 };

   p[120].r=1;
   p[120].fix_rotation(20);
   p[250].fix_rotation(-20);
   p[250].r=1;

while (t<maxtime){

		//wall.predict(dt);
	for(int i=0; i<N; i++){
		p[i].predict(dt);
		}
	
	cal_forces(p);

		//wall.update ...(dt);
	for(int i=0; i<N; i++){
		p[i].update_temp_accel();
	}

	//wall.correct();
	for(int i=0; i<N; i++){
		p[i].correct();
		}

	//wall.update...
	for(int i=0; i<N; i++){
		p[i].update_accel();
	}
	if(fabs(t-1)<0.0001){
		for(int i=0; i<N; i++){
			p[i].mu=150;
		}
		}
	t+=dt;
	output(p);
	}

return 0;
}
