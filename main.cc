#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"
#include"MersenneTwister.h"
#include"wall.h"
#include"celllist.h"


bool cell_list_on=true;
//origin
const vec2d O(0,0);
//unit vectors
const vec2d ux(1,0);
const vec2d uy(0,1);

CCellList grid;

using namespace std;

long RNGSeed;
extern MTRand rgen;

double t=0;
double maxtime=5;
double dt=0.0001;
double outDt=0.01;
vec2d G(0,-10);

size_t N;

ofstream logtime("logtime");

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
	
		if(!cell_list_on)
			for(int j=0; j<i; j++){
			p[j].interact(p[i]);
			}

		}
	
		if(cell_list_on){
			grid.update(p, N);
			grid.interact();
			}
	}

void output(CParticle *p){
        static int count=0, outN=0,outPutN=outDt/dt;
        static ofstream out;

        //this is for a messure of performance

        static double starttime=clock();
	if((10*count)%outPutN==0)
        	logtime<<(clock()-starttime)/CLOCKS_PER_SEC<< "   "<<t<<endl;


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

CParticle *p;
void Initialize(){
/*
	wall.add_segment(0,0,L,0);
	wall.add_segment(0,0,0,L);
	wall.add_segment(L,0,L,L);
	wall.add_segment(0,L,L,L);
*/

	wall.add_line(O,ux);
	wall.add_line(O,uy);
	wall.add_line(ux+uy,-ux);
	wall.add_line(ux+uy,-uy);

	double r=0.003;
	double r_var=0.2;
	grid.setup(O, ux+uy, 2.5*r*(1+0.5*r_var));
	N=grid.nx*grid.ny;
	p=new CParticle[N];
	cerr<< "Number of Paeticles: "<<N <<endl;
	dt=0.01*r;

	//INIT
	double v0=5*r;
	 for(size_t i=0;i<grid.nx;i++){
	  for(size_t j=0;j<grid.ny;j++){

	   p[i*grid.ny+j].set_r(r*(1+r_var*(0.5-drand48())));
	   p[i*grid.ny+j].set_pos(vec2d((i+0.5)*grid.dx, (j+0.5)*grid.dy));

	  double theta=(rand()%100001)*M_PI*2.0e-5;
	   p[i*grid.ny+j].set_vel(vec2d(v0*cos(theta), v0*sin(theta)));
	   grid.add(p[i*grid.ny+j]);
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
void Shutdown(){
	delete [] p;
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
	Shutdown();
	return 0;
	} catch(CException e)
	{
	e.Report();
	return 1;
	}
}
