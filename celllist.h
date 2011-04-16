#ifndef CELLLIST_H
#define CELLLIST_H 
#include<list>
#include"exception.h"
using namespace std;

class CCell : public list<CParticle *>{


	public:
	CCell(){
		//adding itself to the list 
		//of neighbors
		neighs[0]=this;
		}
	void interact(){
	
		CCell:iterator it1, it2;
		for(it1=this->begin(); it1!=this->end(); it1++){
			for(it2=this->begin(); it2!=it1; it2++){
				//for the same cell
				(*it1)->interact(**it2);
				}

			for(int k=1; k<5; k++){
				//for neighbouring cells
				if(neighs[k]==NULL)continue;
				(*it1)->shift(shifts[k]);
				for(it2=neighs[k]->begin(); it2!=neighs[k]->end(); it2++){
					(*it1)->interact(**it2);
					}
				(*it1)->shift(-shifts[k]);
				}
			}
		}

	void add(CParticle *p){
		push_back(p);
		}
	void set_top_right(CCell *c, const vec2d &shift=vec2d(0,0)){
		neighs[1]=c;
		shifts[1]=shift;
		}
	void set_right(CCell *c, const vec2d &shift=vec2d(0,0)){
		neighs[2]=c;
		shifts[2]=shift;
		}
	void set_bottom_right(CCell *c, const vec2d &shift=vec2d(0,0)){
		neighs[3]=c;
		shifts[3]=shift;
		}
	void set_bottom(CCell *c, const vec2d &shift=vec2d(0,0)){
		neighs[4]=c;
		shifts[4]=shift;
		}

	CCell *top_right(){
		return neighs[1];
		}
	CCell *right(){
		return neighs[2];
		}
	CCell *bottom_right(){
		return neighs[3];
		}
	CCell *bottom(){
		return neighs[4];
		}

	private:
	CCell *neighs[5];
	vec2d shifts[5];
	};


/*
neighorhood numbering
each cell track only 
half of its neighbors
	 -----------
	|   |   | 1 |
	 -----------
	|   | 0 | 2 |
	 -----------
	|   | 4 | 3 |
	 -----------
*/

class CCellList
	{
	public:
	CCellList():nodes(NULL),periodic_x(true),periodic_y(true) {
		}

	~CCellList(){
		delete [] nodes;
		}
	void setup(const vec2d &_c, const vec2d &_diag, double d){
		c=_c;
		diag=_diag;
		cerr<< "Constructing the grid ... ";
		nx=floor(diag(0)/d);
		ny=floor(diag(1)/d);
		dx=diag(0)/(double)nx;
		dy=diag(1)/(double)ny;
		if(nodes!=NULL)delete [] nodes;
		nodes=new CCell[nx*ny];
		build_neighbors();
		cerr<< "done: "<<nx<<" X "<<ny <<endl;
		}
	void clear(){
		for(int i=0; i<nx*ny; i++){
		nodes[i].clear();
		}
		
		}
	void update(CParticle *p, size_t N){
		clear();
		for(int i=0; i<N; i++){
			add(p[i]);
			}
		}
	CCell *which(CParticle &particle){
		vec2d xp=particle.get_x()-c;
		int i=(int)(floor(xp(0)/dx));
		int j=(int)(floor(xp(1)/dy));
		if(!periodic_x) ERROR(i<0 or i>=nx, "Point out of grid: "+stringify(particle.get_x())+stringify(i)+" " +stringify(j));
		if(!periodic_y) ERROR(j<0 or j>=ny, "Point out of grid: "+stringify(particle.get_x())+stringify(i)+" " +stringify(j));
		vec2d shift(0,0);
		CCell *p=boundary_mask(i,j, shift);
		particle.shift(shift);
		return p;
		}

	void add(CParticle &p){
		CCell *c=which(p);
		c->add(&p);
		}
	void interact(){
		int i;
		#pragma omp parallel 
		{
		#pragma omp for 
		for(i=0; i<nx*ny; i++){
			nodes[i].interact();
			}
		}
		}

	void print(ostream &out=cout)const{
		out<< "lines"<<endl;
		double x=c(0);
		while(x<=c(0)+diag(0)+1e-10){
			out<< x<<"  "<< c(1)<<"\t"<< x <<"  "<<c(1)+diag(1)<<endl;
			x+=dx;
			}
		double y=c(1);
		while(y<=c(1)+diag(1)+1e-10){
			out<< c(0)<<"  "<< y<<"\t"<< c(0)+diag(0)<<"  "<<y<<endl;
			y+=dy;
			}
		}

	CCell *boundary_mask(int i,int j, vec2d &shift);
	CCell *node(int i, int j)const{
		return &nodes[ny*i+j];
		}
 	//private:
	void build_neighbors();
	int nx, ny;
	vec2d c, diag;
	double dx, dy;
	CCell *nodes;
 	private:
	bool periodic_x, periodic_y;
	};


CCell *CCellList::boundary_mask(int i,int j, vec2d &shift){
	
		shift=0;
		if(i >= nx){
			if(periodic_x){
				i-=nx;
				shift(0)-=diag(0);
				}
			else return NULL;
			}
		if(i < 0){
			if(periodic_x){
				i+=nx;
				shift(0)+=diag(0);
				}
			else return NULL;
			}
		if(j >= ny){
			if(periodic_y){
				j-=ny;
				shift(1)-=diag(1);
				}
			else return NULL;
				}
		if(j < 0){
			if(periodic_y){
				j+=ny;
				shift(1)+=diag(1);
				}
			else return NULL;
				}
	
		return node(i,j);
		}

//i=0, j=0 is top left cell of the grid
//i=nx-1, j=ny-1 is bottom right cell of the grid
void CCellList::build_neighbors(){
	for(int i=0; i<nx; i++){
	for(int j=0; j<ny; j++){
		vec2d shift(0,0);	
		CCell *p;
		p=boundary_mask(i-1,j+1, shift);
		node(i,j)->set_top_right(p, shift);

		p=boundary_mask(i+1,j, shift);
		node(i,j)->set_bottom(p, shift);
		
		p=boundary_mask(i,j+1, shift);
		node(i,j)->set_right(p, shift);

		p=boundary_mask(i+1,j+1, shift);
		node(i,j)->set_bottom_right(p, shift);
		}
		}
	}

#endif /* CELLLIST_H */
