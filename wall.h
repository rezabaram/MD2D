#ifndef WALL_H
#define WALL_H 
#include<vector>
#include"particle.h"
using namespace std;

#define sgn(a) ((a) > 0) ? (1.0) : (-1.0)
class CLineSegment{
	public:
	CLineSegment(double _x1, double _y1, double _x2, double _y2){
		x1=_x1;
		x2=_x2;
		y1=_y1;
		y2=_y2;
		}

	void interact(CParticle &p){
		double wl=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

		double tx=(x2-x1)/wl,ty=(y2-y1)/wl;
		double nx=ty,ny=-tx;
		double qx=(p.x-x1),qy=(p.y-y1);
		int sg = 1;

		double l=(qx*tx + qy*ty);
		double d;

		if(l<0){
			d=sqrt(qx*qx + qy*qy);
			if( d > p.r)return;
			nx=qx/d; ny=qy/d;
		}else{

			if(l>wl){
				qx=(p.x-x2); qy=(p.y-y2);
				d=sqrt(qx*qx + qy*qy);
				if( d > p.r)return;
				nx=qx/d; ny=qy/d;
			}else{
				d=fabs(qx*nx + qy*ny);
				if( d > p.r)return;
				sg=sgn(qx*nx + qy*ny);
			};
		};

		double ovl=fabs(d - p.r);	//double vn=p.vy;
   		double fn=sg*p.kn*pow(ovl,1.5);
		p.fx+=fn*nx; p.fy+=fn*ny;

		}

	double x1, x2;
	double y1, y2;
	};
class CWall
	{
	public:
	CWall(){}

	void add_segment(double _x1, double _y1, double _x2, double _y2){
		add_segment(CLineSegment(_x1, _y1, _x2, _y2));
		}

	void add_segment(const CLineSegment &seg){
		segments.push_back(seg);
		}
	void interact(CParticle &p){
		for(int i=0; i<segments.size(); i++){
			segments.at(i).interact(p);
			}
		}
	
 	private:
	vector<CLineSegment> segments;
	
	};

#endif /* WALL_H */
