#ifndef MDSYS_H
#define MDSYS_H 
#include"tools.h"
#include"config.h"
#include"particle.h"
#include"MersenneTwister.h"
#include"wall.h"
#include"celllist.h"
using namespace std;
extern MTRand rgen;

CConfig config("config");

double Lx=config.get_param<double>("Lx");
double Ly=config.get_param<double>("Ly");
double r=config.get_param<double>("size");
double maxtime=config.get_param<double>("maxtime");
double outStart=config.get_param<double>("outStart");
double outEnd=config.get_param<double>("outEnd");
double outDt=config.get_param<double>("outDt");
double wall_pressure=config.get_param<double>("wall_pressure");
vec2d G=config.get_param<vec2d>("G");

bool periodic_x=config.get_param<bool>  ("periodic_x");
bool periodic_y=config.get_param<double>("periodic_y");



/* Defining basic vectors ----------------------*/
//origin
const vec2d O(0,0);
//unit vectors
const vec2d ux(1,0);
const vec2d uy(0,1);


/* Grid ----------------------------------------*/
bool cell_list_on=true;
bool print_grid_on=false and cell_list_on;
CCellList grid;


/* Wall --------------------------------------- */
CWall wall;
bool wall_moving=true;
//initial velocity of the wall
//this is used mainly to give a tangential velocity
vec2d v0_wall=1.5*ux;
double v_max_wall=0.2;
double shear=0.0;
double work_by_wall=0;


/* Simulation ---------------------------------*/
CParticle *p;
double t=0;
int N;
double r_var=config.get_param<double>("r_var");
double exponent=2.4;
ofstream logtime("logtime");
double dt=0.01*r/2;
double v0=1*r;



double cal_energy(CParticle *p){

	double e=0;
	int i;
	//#pragma omp parallel for
	for(i=0; i<N; i++){
		e+=p[i].energy();
		e-=p[i].m*G*p[i].get_x();
		}

	return e;
	}

void reset_forces(CParticle p[], bool save_contacts=false){
	wall.set_pressure(wall_pressure, shear);
	for(int i=0; i<N; i++){
		//wall.set_force(-wall_pressure*Ly*uy);
		p[i].set_f(p[i].m*G);
		p[i].set_tq(0);
		p[i].contacts.clear();
		p[i].save_contacts=save_contacts;

	}
}
void cal_forces(CParticle p[], bool save_contacts=false){

	
	reset_forces(p, save_contacts);
	for(int i=0; i<N; i++){

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

vec2d center(CParticle *p){

	vec2d cm=0;
	for(int i=0; i<N; i++){
		cm+=p[i].get_x();
		}
	cm*=(1./(double)N);
	return cm;
	}


void output(CParticle *p){
        static int count=0, outN=0,outPutN=outDt/dt;
        static ofstream out;

        //this is for a messure of performance

        static double starttime=clock();
	if((10*count)%outPutN==0)
        	logtime<<(clock()-starttime)/CLOCKS_PER_SEC<< "   "<<t<<endl;


        if(count%outPutN==0 and t>=outStart and t<=outEnd ){
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
		//out<<"contacts"<<endl;
		out<<"contacts"<<endl;
		cal_forces(p, true);
		for(int i=0; i<N; i++){
			for(int j=0; j<p[i].contacts.size();j++){
				double diss=p[i].contacts.at(j).dissipation;
				if(fabs(diss)<0.0001 and fabs(p[i].get_w()*p[i].get_r()>0.02))out<< p[i].get_x()<<"  "<<p[i].get_x()+p[i].contacts.at(j).dx<<"   "<<diss<<endl;
				}
			}
		
		out.close();
		count=0;
		outN++;
		cout<< t <<"\t"<< cal_energy(p)<<'\t'<< work_by_wall <<endl;
		work_by_wall=0;
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
		//wall.add_line(O,uy);
		//defining a moving wall
		wall.add_line(Lx*ux+Ly*uy,-uy, wall_moving, v0_wall, v_max_wall);
		//wall.add_line(Lx*ux+Ly*uy,-uy);
		wall.add_line(O,uy, wall_moving, -v0_wall, v_max_wall);
		//wall.add_line(O,uy);
		}
	else if(!periodic_x and !periodic_y){
		wall.add_line(O,ux);
		wall.add_line(Lx*ux+Ly*uy,-ux);
		wall.add_line(O,uy);
		//defining a moving wall
		wall.add_line(Lx*ux+Ly*uy,-uy, wall_moving, v0_wall, v_max_wall);
		}
	else{
		ERROR(1,"The boundary configuration not implemented.");
		}
/* ------------------------------------------------*/

	grid.setup(O, Lx*ux+Ly*uy, 2.1*r*(1+0.5*r_var));
	N=grid.nx*grid.ny;
	p=new CParticle[N];
	cerr<< "Number of Paeticles: "<<N <<endl;
	 for(int i=0;i<grid.nx;i++){
	  for(int j=0;j<grid.ny;j++){

	   //double radius=r*(1+r_var*(0.5-drand48()));
	   double radius=power_law(r*(1-0.5*r_var), r*(1+0.5*r_var), -exponent);
	   p[i*grid.ny+j].set_r(radius);
	   p[i*grid.ny+j].set_x(vec2d((i+0.5)*grid.dx, (j+0.5)*grid.dy));

	  //double theta=(rand()%100001)*M_PI*2.0e-5;
	   //p[i*grid.ny+j].set_v(vec2d(v0*cos(theta), v0*sin(theta)));
	   p[i*grid.ny+j].set_v(vec2d(0,0));
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
		int i;
		//#pragma omp parallel for
		for(i=0; i<N; i++){
			p[i].predict(dt);
			}
		
		cal_forces(p);

		wall.correct();

		//#pragma omp parallel for
		for(i=0; i<N; i++){
			p[i].correct();
			}

		t+=dt;
		work_by_wall+=wall.work_rate()*dt;
		output(p);
		}
}
void Shutdown(){
	delete [] p;
	}

#endif /* MDSYS_H */
