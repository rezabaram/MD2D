#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"
#include"MersenneTwister.h"
#include"wall.h"

//origin
const vec2d O(0,0);
//unit vectors
const vec2d ux(1,0);
const vec2d uy(0,1);

using namespace std;

long RNGSeed;
extern MTRand rgen;

double t=0;
double maxtime=10;
double dt=0.0001;
double outDt=0.02;
double L=20;
vec2d G(0,-10);

const size_t NL=10;
const size_t NC=10;
const size_t N=NL*NC;

CWall wall;

double cal_energy(CParticle *p){

	double e=0;
	for(int i=0; i<N; i++){
		e+=p[i].energy();
		e-=p[i].m*G*p[i].x;
		}

	return e;
	}

void cal_forces(CParticle p[]){

	
	for(int i=0; i<N; i++){
		p[i].f=G*p[i].m;
		p[i].tq=0;

		wall.interact(p[i]);

		for(int j=0; j<i; j++) {
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

CParticle p[N];
void Initialize(){
	cerr<< N <<endl;
/*
	wall.add_segment(0,0,L,0);
	wall.add_segment(0,0,0,L);
	wall.add_segment(L,0,L,L);
	wall.add_segment(0,L,L,L);
*/

	wall.add_line(O,ux);
	wall.add_line(O,uy);
	wall.add_line(L*ux+L*uy,-ux);
	wall.add_line(L*ux+L*uy,-uy);

	//INIT
	double v0=10;
	 for(size_t i=0;i<NL;i++){
	  for(size_t j=0;j<NC;j++){
	   double r=L/(double)(NC+1)/2.9;
	   p[i*NC+j].set_r(r*(1+.3*(1-drand48())));
	   p[i*NC+j].set_pos(vec2d(2.0*r+j*(L-2.5*r)/NC +(i%2)*0.5*L/NC, 2.0*r + i*L/NC)) ;
	   double theta=(rand()%100001)*M_PI*2.0e-5;
	   p[i*NC+j].set_vel(vec2d(v0*cos(theta), v0*sin(theta)));
	  }; 
	 };
	

	   //p[120].r=1;
	   //p[120].fix_rotation(20);
	   //p[250].fix_rotation(-20);
	   //p[250].r=1;

	}

void Run(){

	output(p);
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
		t+=dt;
		output(p);
		}
}

int main(int pi, char **params){
	if(pi==1)
		RNGSeed=0;
	else
		RNGSeed=313*atoi(params[1])+1;

	cerr<<"RNG Seed: "<<RNGSeed<<endl;
	
	try {
	Initialize();
	Run();
	//Shutdown();
	return 0;
	} catch(CException e)
	{
	e.Report();
	return 1;
	}
}
