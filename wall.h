#ifndef WALL_H
#define WALL_H 
#include<vector>
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
		p.f+=fn*n; 

		}

	vec2d x1, x2;
	};

class CLine
	//has eq: n.(X-X0)=0;
	{
	public:
	CLine(const vec2d &_x, const vec2d &_n):x(_x), n(_n){
		n.normalize();
		}
	double distance(const vec2d &_x){
		return (_x-x)*n;
		}
	void interact(CParticle &p){
		double d=distance(p.x);
		double ovl=d - p.get_r();	
		if(sgn(ovl)>0)return;
   		double fn=p.kn*pow(fabs(ovl),1.5);
		p.f+=fn*n;
		}

	vec2d x, n;
 	private:
	};

class CWall
	{
	public:
	CWall(){}

	void add_segment(double _x1, double _y1, double _x2, double _y2){
		add_segment(CLineSegment(vec2d(_x1, _y1), vec2d(_x2, _y2)));
		}

	void add_segment(const CLineSegment &seg){
		segments.push_back(seg);
		}
	void add_line(const vec2d &_x,const vec2d &_n){
		lines.push_back(CLine(_x,_n));
		}
	void interact(CParticle &p){
		for(int i=0; i<segments.size(); i++){
			segments.at(i).interact(p);
			}
		for(int i=0; i<lines.size(); i++){
			lines.at(i).interact(p);
			}
		}

	void apply_motion(){
		
		}
	
 	private:
	vector<CLineSegment> segments;
	vector<CLine> lines;
	
	};

#endif /* WALL_H */
