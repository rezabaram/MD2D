#ifndef WALL_H
#define WALL_H 
#include<vector>
#include"object.h"
#include"particle.h"
using namespace std;

#define sgn(a) ((a) > 0) ? (1.0) : (-1.0)
class CLineSegment{
	public:
	CLineSegment(const vec2d & _x1, const vec2d &_x2){
		x1=_x1;
		x2=_x2;
		}

	void interact(CParticle &p){
		vec2d t=(x2-x1);
		double wl=t.abs();
		t*=1.0/wl;//normalize the vector t;

		vec2d n(t(1),-t(0));
		vec2d q=(p.x-x1);
		int sg = 1;

		double l=q*t;//inner product
		double d;

		double r=p.get_r();
		if(l<0){
			d=q.abs();
			if( d > r)return;
			n=q/d; 
		}else{

			if(l>wl){
				q=(p.x-x2); 
				d=q.abs();
				if( d > r)return;
				n=q/d; 
			}else{
				d=fabs(q*n);
				if( d > r)return;
				sg=sgn(q*n);
			};
		};

		double ovl=fabs(d - r);	//double vn=p.vy;
   		double fn=sg*p.kn*pow(ovl,1.5);

		vec2d vr = p.v + r*p.w*t;

		double vr_t = vr*t;

    	double ft=(fn>0)?(-p.mu*fn*vr_t/10.):(p.mu*fn*vr_t/10.);

		vec2d df= fn*n + ft*t;
		p.f+=df;
		if(!p.rotation_fixed) p.tq+=ft*r;
		}

	vec2d x1, x2;
	};

class CLine : public CObject
	//has eq: n.(X-X0)=0;
	{
	public:
	CLine(const vec2d &_x, const vec2d &_n, bool _moving=false):n(_n), moving(_moving){
		x=_x;
		n.normalize();
		t(0)=n(1);
		t(1)=-n(0);
		m=1;//mass
		}
	double distance(const vec2d &_x){
		return (_x-x)*n;
		}
	void interact(CParticle &p){
		double d=distance(p.x);
		double r=p.get_r();
		double ovl=d - r;	
		if(sgn(ovl)>0)return;
   		double fn=p.kn*pow(fabs(ovl),1.5);
   		
		vec2d vr = p.v + r*p.w*t;

		double vr_t = vr*t;

    		double ft=-p.mu*fn*vr_t/10.;

		vec2d df= fn*n + ft*t;
		p.f+=df;
		if(!p.rotation_fixed) p.tq+=ft*r;

		//force on wall
		f-=fn*n;//normal force
		}

	void print(ostream &out=cout)const{
		out<< x-100*t <<"\t"<< x+100*t <<endl;
		}

	vec2d n, t;
	bool moving;
 	private:
	};

class CWall
	{
	public:
	CWall(){}
	~CWall(){
		//cleaning the memory which was dynamcially allocated
		for(int i=0; i<segments.size(); i++){
			delete segments.at(i);
			}
		for(int i=0; i<lines.size(); i++){
			delete lines.at(i);
			}
		}

	void add_segment(double _x1, double _y1, double _x2, double _y2){
		add_segment(new CLineSegment(vec2d(_x1, _y1), vec2d(_x2, _y2)));
		}

	void add_segment(CLineSegment *seg){
		segments.push_back(seg);
		}

	void add_line(const vec2d &_x,const vec2d &_n, bool moving=false){
		lines.push_back(new CLine(_x,_n, moving));
		}

	void interact(CParticle &p){
		for(int i=0; i<segments.size(); i++){
			segments.at(i)->interact(p);
			}
		for(int i=0; i<lines.size(); i++){
			lines.at(i)->interact(p);
			}
		}
	void print(ostream &out=cout)const{
		for(int i=0; i<lines.size(); i++){
			lines.at(i)->print(out);
			}
		}
	void predict(double dt){
		for(int i=0; i<lines.size(); i++){
			//if(lines.at(i)->moving)lines.at(i)->f=vec2d(0,-10);
			if(lines.at(i)->moving)lines.at(i)->predict(dt);
			}
		}
	void update_accel(){
		for(int i=0; i<lines.size(); i++){
			if(lines.at(i)->moving)lines.at(i)->update_accel();
			}
		}
	void correct(){
		for(int i=0; i<lines.size(); i++){
			if(lines.at(i)->moving)lines.at(i)->correct();
			}
		}

	void apply_motion(){
		
		}
	
 	private:
	vector<CLineSegment*> segments;
	vector<CLine*> lines;
	
	};

#endif /* WALL_H */
