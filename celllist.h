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
				for(it2=neighs[k]->begin(); it2!=neighs[k]->end(); it2++){
					(*it1)->interact(**it2);
					}
				}
			}
		}

	void add(CParticle *p){
		push_back(p);
		}
	void set_top_right(CCell *c){
		neighs[1]=c;
		}
	void set_right(CCell *c){
		neighs[2]=c;
		}
	void set_bottom_right(CCell *c){
		neighs[3]=c;
		}
	void set_bottom(CCell *c){
		neighs[4]=c;
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
	CCellList(){}

	~CCellList(){
		delete [] nodes;
		}
	void setup(const vec2d &_c1, const vec2d &diag, double d){
		c1=_c1;
		c2=_c1+diag;
		cerr<< "Constructing the grid ... ";
		nx=floor(diag(0)/d);
		ny=floor(diag(1)/d);
		dx=diag(0)/(double)nx;
		dy=diag(1)/(double)ny;
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
	CCell &which(const vec2d &x){
		vec2d xp=x-c1;
		int i=xp(0)/dx;
		int j=xp(1)/dy;
		ERROR(i<0 or i>=nx, "Point out of grid: "+stringify(x)+stringify(i)+" " +stringify(j));
		ERROR(j<0 or j>=ny, "Point out of grid: "+stringify(x)+stringify(i)+" " +stringify(j));
		return nodes[ny*i+j];
		}

	void add(CParticle &p){
		which(p.x).add(&p);
		}
	void interact(){
		for(int i=0; i<nx*ny; i++){
			nodes[i].interact();
			}
		}

	CCell &node(size_t i, size_t j)const{
		return nodes[ny*i+j];
		}
 	//private:
	void build_neighbors();
	size_t nx, ny;
	vec2d c1, c2;
	double dx, dy;
	CCell *nodes;
 	private:
	};

//i=0, j=0 is top left cell of the grid
//i=nx-1, j=ny-1 is bottom right cell of the grid
void CCellList::build_neighbors(){
	for(int i=0; i<nx; i++){
	for(int j=0; j<ny; j++){

		if(i>0 and j<ny-1)node(i,j).set_top_right(&node(i-1,j+1));
		else node(i,j).set_top_right(NULL);

		if(i<nx-1)node(i,j).set_bottom(&node(i+1,j));
		else node(i,j).set_bottom(NULL);

		if(j<ny-1)node(i,j).set_right(&node(i,j+1));
		else node(i,j).set_right(NULL);


		if(i<nx-1 and j<ny-1)node(i,j).set_bottom_right(&node(i+1,j+1));
		else node(i,j).set_bottom_right(NULL);
		}
		}
	}

#endif /* CELLLIST_H */
