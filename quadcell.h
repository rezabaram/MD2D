#ifndef QUADCELL_H
#define QUADCELL_H 
#include<list>
using namespace std;

class CQuadCell {
	public:
	CQuadCell(const vec2d &_c, const vec2d &_L, int g=0):c(_c), L(_L), capacity(2), gen(g),nParticles(0){
		split=false;
		stop=true;
		for(int i=0; i<4; i++){
			child[i]=NULL;
			}
		}
	~CQuadCell(){
		fullclear();
		}

	void shallowclear(){
		stop=true;
		nParticles=0;
		contact.exist=false;
		}
	void fullclear(){
		if(split)
		for(int i=0; i<4; i++){
			delete child[i];
			child[i]=NULL;
			}
		split=false;
		stop=true;
		}
	
	void cal_interactions(){
		if(!stop) {
			assert(split);
			for(int i=0; i<4; i++)
				child[i]->cal_interactions();
			//return;
			}
		assert(nParticles<=2);
		if(nParticles!=2)return;
		//if(!contact.exist)return;
		contact.calculate();
		if( !contact.exist)return;
		if( !this->contains(contact.x))return;
		assert( contact.p1!=contact.p2);
		contact.apply_forces();
		}
	
		/*
		 --------
		| 1 | 2 |
		 --------
		| 0 | 3 |
		 --------
		*/
	void give_over_down(CParticle *p){
		assert(split);
		for(int i=0; i<4; i++)
			child[i]->add(p);
		}
	void refine(){
		ERROR(gen==20,"Refinement limit reached: "+stringify(20,2));
		if(!split){//allocate only if it is not
			vec2d L2=L/2.0;
			child[0]= new CQuadCell(c,L2, gen+1);
			child[1]= new CQuadCell(c+vec2d(0,L2(1)),L/2., gen+1);
			child[2]= new CQuadCell(c+L2,L2, gen+1);
			child[3]= new CQuadCell(c+vec2d(L2(0),0),L/2., gen+1);
			}

		child[0]->shallowclear();
		child[1]->shallowclear();
		child[2]->shallowclear();
		child[3]->shallowclear();
		split=true;
		stop=false;
		
		//give the particles over to the childern
		if(nParticles>=1)give_over_down(contact.p1);
		if(nParticles==2)give_over_down(contact.p2);
	
		assert(nParticles<=2);
		nParticles=0;
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

	template<class T>
	 T clamp(T X, T Min, T Max)const {
	   return ( X > Max ) ? Max : ( X < Min ) ? Min : X;
	 }
	
	bool contains (const vec2d &x){
		return ( x(0)>c(0) and x(1)>c(1) and x(0)<c(0)+L(0) and x(1)<c(1)+L(1) );
		}

	bool intersect(const CParticle *p)const{
		vec2d center=p->get_x();
		double radius=p->get_r();
		vec2d closest(clamp(center(0), c(0), c(0)+L(0)), clamp(center(1), c(1), c(1)+L(1)));    
		return (closest - center).abs2() <= radius * radius+1e-10;
		}

	bool add(CParticle *p){
		if(!intersect(p))return false;
		if(nParticles==2) {
			refine();
			give_over_down(p);
			return true;
			}
		if(!stop){ 
			assert(split);
			give_over_down(p);
			return true;
			}
		if(nParticles==1) {contact.p2=p; ++nParticles;}
		if(nParticles==0) {contact.p1=p; ++nParticles;}

		return true;
		}

	void print(ofstream &outf, string s="")const;

	CQuadCell *child[4];
	vec2d c, L;
	bool split, stop;
	unsigned int capacity;
	int gen,nParticles;
	CParticle::CContact contact;
	
	};

void CQuadCell::print(ofstream &outf, string s)const{
	if(gen==0){
		outf<<"lines"<<endl;
		outf<< c <<"\t"<< c+vec2d(L(0),0)<<endl;
		outf<< c+vec2d(L(0),0) <<"\t"<< c+L<<endl;
		outf<< c+L <<"\t"<< c+vec2d(0,L(1))<<endl;
		outf<< c+vec2d(0,L(1))<<"\t"<<c<<endl;
		}

	if(split){
			assert(split);
			for(int i=0;i<4;i++){
				outf<<s<< c+vec2d(0,L(1)/2) <<"\t" <<c+vec2d(L(0),L(1)/2) <<endl;
				outf<<s<< c+vec2d(L(0)/2,0) <<"\t" <<c+vec2d(L(0)/2,L(1)) <<endl;
				child[i]->print(outf, s+"         ");
			 }
		}
	else{
		//drawings for debugging
		return;
		if(nParticles==2) {
			contact.print(outf);
			}
		return;
			//vec2d center=(*it)->get_x();
			//vec2d temp(clamp(center(0), c(0), c(0)+L(0)), clamp(center(1), c(1), c(1)+L(1)));    
			//outf<<center<<"   "<<temp<<endl;
			//outf<< center <<"\t" <<c+L/2. <<endl;
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
