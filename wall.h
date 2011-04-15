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
		vec2d q=(p.get_x()-x1);
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
				q=(p.get_x()-x2); 
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

		vec2d vr = p.get_v() + r*p.get_w()*t;

		double vr_t = vr*t;

    	double ft=(fn>0)?(-p.mu*fn*vr_t/10.):(p.mu*fn*vr_t/10.);

		vec2d df= fn*n + ft*t;
		p.add_f(df);
		if(!p.rotation_fixed) p.add_tq(ft*r);
		}

	void print(ostream &out=cout)const{
		out<< x1 <<"\t"<< x2 <<endl;
		}

	vec2d x1, x2;
	};

class CLine : public CObject<2>
	//has eq: n.(X-X0)=0;
	{
	public:
	CLine(const vec2d &_x, const vec2d &_n, bool _moving, const vec2d &_v0, double maxv=1e+10):n(_n), moving(_moving){
		x=_x;
		v=_v0;
		n.normalize();
		t(0)=n(1);
		t(1)=-n(0);
		m=.1;//mass
		v_max=maxv;
		}
	void set_const_force(const vec2d &_f){
		const_f=_f;
		f=_f;
		}

	void reset_const_force(){
		f=const_f;
		}

	void check_max(){
		//imposing a maximum on the normal 
		//component of the velocity
		double d=fabs(v*n);
		if(d>v_max){
			vec2d vn=n*(v*n);
			v-=vn;
			vn*=v_max/d;
			v+=vn;
			}
		}

	double distance(const vec2d &_x){
		return (_x-x)*n;
		}
	virtual void interact(CParticle &p){
		double d=distance(p.get_x());
		double r=p.get_r();
		double ovl=d - r;	
		if(sgn(ovl)>0)return;
   		
		vec2d vr = p.get_v() + r*p.get_w()*t - v;
   		//double fn=p.kn*pow(fabs(ovl),1.5);
   		double fn=p.kn*pow(fabs(ovl),1.5);//+p.kd*(vr*n)*sqrt(ovl);

		double vr_t = vr*t;

    		double ft=-p.mu*fn*vr_t/10.;

		vec2d df= fn*n + ft*t;
		p.add_f(df);
		if(!p.rotation_fixed) p.add_tq(ft*r);

		//force on =wall
		f-=fn*n;//normal force
		//f-=df;
		}

	void print(ostream &out=cout)const{
		out<< x-100*t <<"\t"<< x+100*t <<endl;
		}

	vec2d n, t;
	vec2d const_f;
	bool moving;
 	private:
	};


class CShadowLine: public CLine{
	public:
	CShadowLine(const vec2d &_x, const vec2d &_n):CLine(_x, _n, false, vec2d(0,0)){};
	void interact(CParticle &p){
		return;
		}
	
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

	void add_shadow_line(const vec2d &_x,const vec2d &_n){
		lines.push_back(new CShadowLine(_x,_n));
		}
	void add_line(const vec2d &_x,const vec2d &_n, bool moving=false, const vec2d &_v0=vec2d(0,0), double v_max=1e+14){
		lines.push_back(new CLine(_x,_n, moving, _v0, v_max));
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
			CLine &line=*lines.at(i);
			if(line.moving){
				line.predict(dt);
				//if(line.x(1)>1)line.x(1)=.9999;
				//if(line.x(1)<0)line.x(1)=0;
				}
			}
		}
	void correct(){
		for(int i=0; i<lines.size(); i++){
			if(lines.at(i)->moving)lines.at(i)->correct();
			}
		}

	void apply_motion(){
		
		}
	
	void set_pressure(double p, double shear){
		for(int i=0; i<lines.size(); i++){
			//if(lines.at(i)->moving)cerr<<  lines.at(i)->f<<endl;
			if(lines.at(i)->moving)lines.at(i)->set_f(vec2d(p*lines.at(i)->n- shear*lines.at(i)->t));
			}
		}

	void set_force(const vec2d &g){
		for(int i=0; i<lines.size(); i++){
			if(lines.at(i)->moving)lines.at(i)->set_f(g);
			}
		}
 	private:
	vector<CLineSegment*> segments;
	vector<CLine*> lines;
	
	};

#endif /* WALL_H */
