#ifndef QUADCELL_H
#define QUADCELL_H 
#include<list>
using namespace std;

class CQuadCell :  public list<CParticle *>{
	public:
	CQuadCell(const vec2d &_c, const vec2d &_L, int g=0):c(_c), L(_L), capacity(2), gen(g){
		split=false;
		for(int i=0; i<4; i++){
			child[i]=NULL;
			}
		}
	~CQuadCell(){
		if(split)
		for(int i=0; i<4; i++){
			delete child[i];
			child[i]=NULL;
			}
		}
	
	
	void refine(){
		/*
		 --------
		| 1 | 2 |
		 --------
		| 0 | 3 |
		 --------
		*/
		vec2d L2=L/2.0;
		child[0]= new CQuadCell(c,L2, gen+1);
		child[1]= new CQuadCell(c+vec2d(0,L2(1)),L/2., gen+1);
		child[2]= new CQuadCell(c+L2,L2, gen+1);
		child[3]= new CQuadCell(c+vec2d(L2(0),0),L/2., gen+1);
		split=true;
		
		//give the particles over to the childern
		CQuadCell::iterator it;
		bool success=false;
		for(it=this->begin(); it!=this->end(); it++){
			success=(child[0]->add(*it) or child[1]->add(*it) or child[2]->add(*it) or child[3]->add(*it));
			}
		ERROR(!success, "Failure in refinemnet of a cell");
		this->clear();
		}

	
	void add(CParticle *p, int N){
		for(int i=0; i<N; i++){
			add(&p[i]);
			}
		}
	void add(list<CParticle *> &l){
		list<CParticle *>::iterator it;
		for(it=l.begin(); it!=l.end(); it++){
			add(*it);
			}
		}

	bool add(CParticle *p){
		vec2d x=p->get_x();
		double r=p->get_r();
		//if( x(0)+r<c(0) or  x(1)+r<c(1) or x(0)-r>c(0)+L(0) or  x(1)-r>c(1)+L(1) ) return false;
		if( x(0)+r<c(0) or  x(1)+r<c(1) or x(0)-r>c(0)+L(0) or  x(1)-r>c(1)+L(1) ) return false;
		if(this->size()==capacity) if(!split) refine();
		if(split) for(int i=0; i<4; i++){
			child[i]->add(p);
			}
		else{
			this->push_back(p);
			}
		return true;
		ERROR(1,"Should not reach here");
		}
	void print(ofstream &outf, string s="")const;

	CQuadCell *child[4];
	vec2d c, L;
	bool split;
	unsigned int capacity;
	int gen;
	};

void CQuadCell::print(ofstream &outf, string s)const{
	if(gen==0){
		outf<<"lines"<<endl;
		outf<< c <<"\t"<< c+vec2d(L(0),0)<<endl;
		outf<< c+vec2d(L(0),0) <<"\t"<< c+L<<endl;
		outf<< c+L <<"\t"<< c+vec2d(0,L(1))<<endl;
		outf<< c+vec2d(0,L(1))<<"\t"<<c<<endl;
		}

	if(split)for(int i=0;i<4;i++){
		outf<<s<< c+vec2d(0,L(1)/2) <<"\t" <<c+vec2d(L(0),L(1)/2) <<"\t"<< c+L<<endl;
		outf<<s<< c+vec2d(L(0)/2,0) <<"\t" <<c+vec2d(L(0)/2,L(1)) <<"\t"<< c+L<<endl;
		child[i]->print(outf, s+"         ");
		}
	}

void setup_neighbourhood(){
/*	
	if(!split)return;
	vec2d shift(0,0);	
	CQuadCell *p;
	p=boundary_mask(i-1,j+1, shift);
	child[0]->set_top(child[1]);
	child[0]->set_right(child[3]);
	child[0]->set_top_right(child[2]);

	child[1]->set_top(child[1]);
	child[1]->set_right(child[3]);
	child[1]->set_top_right(child[2]);

	child[2]->set_top(child[1]);
	child[2]->set_right(child[3]);
	child[2]->set_top_right(child[2]);

	child[3]->set_top(child[1]);
	child[3]->set_right(child[3]);
	child[3]->set_top_right(child[2]);

*/
	}

#endif /* QUADCELL_H */
