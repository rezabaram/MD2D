#ifndef OBJECT_H
#define OBJECT_H 
#include"vec.h"
#include"dfreedom.h"
#include"mutex_lock.h"

//This is the class from which any moving 
//object is derived. (Putting together the 
//common part, not to repeat it for every object.)



//The template and trait stuffs are just for future in case we want 
//to extend this to 3d as smooth as possible

template<int Dim>
class DimensionTrait{
	};
template<>
class DimensionTrait<2>{
	public:
	typedef vec2d TranslationVariablesType;
	typedef double RotationVariablesType;
	};

template<int Dim=2, class DimTrait=DimensionTrait<Dim> >
class CObject 
	{
	protected:
	typedef typename DimTrait::TranslationVariablesType xType;
	typedef typename DimTrait::RotationVariablesType wType;
	public:
	CObject():translation(x,v,a), rotation(q, w, aq){

		x=0; v=0; a=0;
		q=0; w=0; aq=0;
		f=0; tq=0;
		m=1; Im=1;
		v_max=1e+14;
		}
	
	void predict(double dt){ // (Beeman's algorithm)
		translation.predict(dt);
		rotation.predict(dt);
		if (q>2.0*M_PI) q-=2.0*M_PI; if (q<0) q+=2.0*M_PI;
		}


	void correct(){
		translation.update_accel(f/m);
		rotation.update_accel(tq/Im);
		translation.correct();
		rotation.correct();
		apply_constrains();
		}
	virtual void apply_constrains(){
		}
	virtual void shift(const xType &_x){
		x+=_x;
		}

	wType get_w()const{
		return w;
		}
	wType get_q()const{
		return q;
		}
	void set_w(const wType _w){
		w=_w;
		}
	void set_q(const wType _q){
		q=_q;
		}

	xType get_x()const{
		return x;
		}

	xType get_v()const{
		return v;
		}
	double get_v_max()const{
		return v_max;
		}

	xType get_f()const{
		return f;
		}

	void set_x(const xType &_x){
		x=_x; 
		}

	void set_v(const xType &_v){
		v=_v; 
		}
	void set_v_max(double vmax){
		v_max=vmax; 
		}

	void set_f(const xType &_f){
		f=_f;
		}
	void set_tq(const wType _tq){
		tq=_tq;
		}
	void add_f(const xType &_f){
		ScopedLock lck(fmutex);
		f+=_f;
		}
	void add_tq(wType _tq){
		ScopedLock lck(qmutex);
		if(rotation_fixed) return;
		tq+=_tq;
		}

	double work_rate(){
		return f*v;
		}

	double m, Im;
	protected:
	bool rotation_fixed;
 	//private:
	DFreedom<xType> translation;
	DFreedom<double> rotation;
	double q,w,aq, tq;
	xType x,v,a, f;
	double v_max;
	xType xtemp;
	MutexType fmutex;
	MutexType qmutex;
	};
#endif /* OBJECT_H */
