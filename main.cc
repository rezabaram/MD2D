#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include"particle.h"
#include"MersenneTwister.h"
#include"wall.h"
#include"celllist.h"
using namespace std;

long RNGSeed;
extern MTRand rgen;


/* Defining basic vectors ----------------------*/
//origin
const vec2d O(0,0);
//unit vectors
const vec2d ux(1,0);
const vec2d uy(0,1);


/* Grid ----------------------------------------*/
bool cell_list_on=true;
bool print_grid_on=true and cell_list_on;
CCellList grid;
double Lx=0.7;
double Ly=1;
bool periodic_x=true;
bool periodic_y=false;


/* Wall --------------------------------------- */
CWall wall;
bool is_wall_moving=true;
//initial velocity of the wall
//this is used mainly to give a tangential velocity
vec2d v0_wall=1*ux;
double v_max_wall=.2;
double wall_pressure=.1;


/* Simulation ---------------------------------*/
CParticle *p;
double t=0;
double maxtime=50;
double outDt=0.1;
vec2d G(0,0);
size_t N;
double r=0.008;
double r_var=0.1;
ofstream logtime("logtime");
double dt=0.005*r;
double v0=5*r;


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
		wall.set_force(-wall_pressure*Ly*uy);
		p[i].f=p[i].m*G;
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
		out<<"10000000 0 0 0 0 0 0"<<endl;
		out<<"lines"<<endl;
		wall.print(out);
		if(print_grid_on)grid.print(out);
		out<<"circles"<<endl;
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

void Initialize(){

/* Boundary set up -------------------------------*/
	if(periodic_x and periodic_y){
		wall.add_shadow_line(O,ux);
		wall.add_shadow_line(O,uy);
		wall.add_shadow_line(Lx*ux+Ly*uy,-ux);
		wall.add_shadow_line(Lx*ux+Ly*uy,-uy);
		}
	else if(periodic_x and !periodic_y){
		wall.add_shadow_line(O,ux);
		wall.add_shadow_line(Lx*ux+Ly*uy,-ux);
		wall.add_line(O,uy);
		//defining a moving wall
		wall.add_line(Lx*ux+Ly*uy,-uy, is_wall_moving, v0_wall, v_max_wall);
		}
	else if(!periodic_x and !periodic_y){
		wall.add_line(O,ux);
		wall.add_line(Lx*ux+Ly*uy,-ux);
		wall.add_line(O,uy);
		//defining a moving wall
		wall.add_line(Lx*ux+Ly*uy,-uy, is_wall_moving, v0_wall, v_max_wall);
		}
	else{
		ERROR(1,"The boundary configuration not implemented.");
		}
/* ------------------------------------------------*/

	grid.setup(O, Lx*ux+Ly*uy, 2.1*r*(1+0.5*r_var));
	N=grid.nx*grid.ny;
	p=new CParticle[N];
	cerr<< "Number of Paeticles: "<<N <<endl;
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

		wall.predict(dt);
		for(int i=0; i<N; i++){
			p[i].predict(dt);
			}
		
		cal_forces(p);

		wall.update_accel();
		for(int i=0; i<N; i++){
			p[i].update_accel();
		}

		wall.correct();
		for(int i=0; i<N; i++){
			p[i].correct();
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
