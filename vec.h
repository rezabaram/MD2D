//=======================================================================
//======  Class Vec Template  =====================================
// By Reza M. Baram, 2010
//=======================================================================
#ifndef CVEC_H
#define CVEC_H
#include<ostream>
#include<math.h>
#include<assert.h>
#include<complex>
#include"exception.h"

typedef size_t indexType;

#define check_index

#ifdef check_index
#define CHECK_INDEX(i, D) ERROR((i<0 or i>= D), "Index out of range");
#endif 

using namespace std;

template<class T>
class vecElemPolicy{
	public:
	 static T mul (const T &a, const T &b){ return a*b;}
};
template<>
class vecElemPolicy<complex<double> >{
	public:
	 static complex<double> mul (const complex<double> &a, const complex<double> &b){ 
			return a*conj(b);}
};


template<indexType _dim=3, class T=double, typename policy = vecElemPolicy<T> >
class Vec{
 public:
  Vec(const T v[]){
	for(indexType i=0; i<dim; i++){
		x[i]=v[i];
		}
	};
	
        explicit Vec(const T &x0, const T &x1, const T &x2=(T)0, const T &x3=(T)0){

		if(dim>0) x[0]=x0; 
		else return;

		if(dim>1) x[1]=x1; 
		else return;

		if(dim>2) x[2]=x2; 
		else return;

		if(dim>3) x[3]=x3; 
		else return;

		for(indexType i=4; i<dim; i++) x[i]=(T)0;

		}

	  explicit Vec(const T &a=(T)0){
		for(indexType i=0; i<dim; i++){
			x[i]=a;
			}
		};

	  ~Vec(){
		}

	  T &operator[](indexType i){
		CHECK_INDEX(i, dim);
		return x[i];
		};

	  const T &operator[](indexType i)const{
		CHECK_INDEX(i, dim);
		return x[i];
		};

	  T &operator()(indexType i){
		CHECK_INDEX(i, dim);
		return x[i];
		};
	  const T &operator()(indexType i)const {
		CHECK_INDEX(i, dim);
		return x[i];
		};

	  T operator*(const Vec<_dim, T, policy> &p)const;
	  Vec<_dim, T, policy> operator^(const Vec<_dim, T, policy> &p)const;
	  //const Vec<_dim, T, policy> operator*(T a)const ;
	  Vec &operator=(T a);
	  Vec &operator*=(T a);
	  Vec &operator/=(T a);
	  Vec &operator+=(T a);
	  Vec &operator-=(T a);
	  Vec &operator+=(const Vec &p);
	  Vec &operator-=(const Vec &p);
	  Vec operator*(T a)const ;
	  Vec operator/(T a)const ;
	  Vec operator+(const Vec &p)const ;
	  Vec operator-(const Vec &p)const ;
	  Vec operator/(const Vec &p)const;
	  T abs2()const;
	  T abs()const;
	  T distance(const Vec p)const;
	  Vec & normalize();
	  Vec normalized()const;

	  template<indexType _dim2, class U, class p >
	  friend std::ostream & operator<< (std::ostream &out, const Vec<_dim2, U, p> &v);
	  template<indexType _dim2, class U, class p>
	  friend std::istream & operator>>(std::istream &in, Vec<_dim2, U, p> &v);
	static const indexType dim;
	private:
	  T x[_dim];
	};
	
	template<indexType _dim, class T, class policy>
	const indexType Vec<_dim, T, policy>::dim=_dim;



	template<indexType _dim, class T, class policy>
	inline
	  std::ostream & operator<< (std::ostream &out, const Vec<_dim, T, policy> &v){
		out<<v.x[0];
		for(indexType i=1; i<v.dim; i++){
			out<<"  "<<v.x[i];
			}
		return out;
		}

	template<indexType _dim, class T, class policy>
	  std::istream & operator>>(std::istream &in, Vec<_dim, T, policy> &v){
		for(indexType i=0; i<v.dim; i++){
			in>>v.x[i];
			}
		return in;
		}

	//elementvise vector multiplication
	template<indexType _dim, class T, class policy>
	Vec<_dim, T, policy> Vec<_dim, T, policy>::operator^(const Vec<_dim, T, policy> &p)const{
		static Vec<_dim, T, policy> v(0.0);
		for(size_t i=0; i<dim; i++){
			v(i)=x[i]*p[i];
			}
		return v;
		}
	//elementvise vector divition
	template<indexType _dim, class T, class policy>
	Vec<_dim, T, policy> Vec<_dim, T, policy>::operator/(const Vec<_dim, T, policy> &p)const{
		static Vec<_dim, T, policy> v(0.0);
		for(size_t i=0; i<dim; i++){
			v(i)=x[i]/p[i];
			}
		return v;
		}

	template<indexType _dim, class T, class policy>
	inline
	  T Vec<_dim, T, policy>::operator*(const Vec<_dim, T, policy> &p)const{
		T prod=(T)0;
		for(indexType i=0; i<dim; i++){
			prod+=policy::mul(x[i],p.x[i]);
			}
		return prod;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator=(T a){
		for(indexType i=0; i<dim; i++){
			x[i]=a; 
			}
		return *this;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator*=(T a){
		for(indexType i=0; i<dim; i++){
			x[i]*=a; 
			}
		return *this;
		}
	template<indexType _dim, class T, class policy>
	inline
	Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator/=(T a){
		ERROR(a==0,"Devided by zero");
		//if(a==0)throw 1;

		for(indexType i=0; i<dim; i++){
			x[i]/=a; 
			}
		return *this;
		}
	template<indexType _dim, class T, class policy>
	inline
	Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator+=(T a){
		for(indexType i=0; i<dim; i++){
			x[i]+=a; 
			}
		return *this;
		}
	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator-=(T a){
		for(indexType i=0; i<dim; i++){
			x[i]-=a; 
			}
		return *this;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator+=(const Vec<_dim, T, policy> &p){
		for(indexType i=0; i<dim; i++){
			x[i]+=p.x[i];
			}
		return *this;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> &Vec<_dim, T, policy>::operator-=(const Vec<_dim, T, policy> &p){
		for(indexType i=0; i<dim; i++){
			x[i]-=p.x[i];
			}
		return *this;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> Vec<_dim, T, policy>::operator*(T a)const {
		Vec<_dim, T, policy> pp(*this);
			pp*=(a);
		return pp;
		}


	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> Vec<_dim, T, policy>::operator/(T a)const {
		Vec<_dim, T, policy> pp(*this);
			pp/=(a);
		return pp;
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy>Vec<_dim, T, policy>::operator+(const Vec<_dim, T, policy> &p)const {
		Vec<_dim, T, policy> pp(*this);
		pp+=p;
		return pp;
		}


	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> Vec<_dim, T, policy>::operator-(const Vec<_dim, T, policy> &p)const {
		Vec<_dim, T, policy> pp(*this);
		pp-=p;
		return pp;
		}

	template<indexType _dim, class T, class policy>
	inline
	  T Vec<_dim, T, policy>::abs2()const{
		return (*this)*(*this);
		}

	template<indexType _dim, class T, class policy>
	inline
	  T Vec<_dim, T, policy>::abs()const{
		return sqrt(abs2());
		}

	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> & Vec<_dim, T, policy>::normalize(){
		T d=abs();
		ERROR(d==0, "Normalizing a vector with zero lenth" );
		d=1/d;
		
		(*this)*=d;
		return *this;
		}
	template<indexType _dim, class T, class policy>
	inline
	  Vec<_dim, T, policy> Vec<_dim, T, policy>::normalized()const{
		return Vec<_dim, T, policy> (*this).normalize();
		}

	template<indexType _dim, class T, class policy>
	inline
	  const Vec<_dim, T, policy> operator *(double a, Vec<_dim, T, policy> v){
		v*=(a);
		return v;
		};

	template<indexType _dim, class T, class policy>
	inline
	  const Vec<_dim, T, policy> operator -(Vec<_dim, T, policy> v){
		v*=(T)(-1.0);
		return v;
		};

	//template<>
	//Vec::Vec(double , double, double){}

	template<indexType _dim, class T, class policy>
	inline
	Vec<3, T, policy> cross(const Vec<_dim, T, policy> u, const Vec<_dim, T, policy> v){
		//only first 3 components
		assert(u.dim>=3 and v.dim>=3);
		return Vec<3, T, policy> (policy::mul(u(1),v(2))-policy::mul(u(2),v(1)),policy::mul(u(2),v(0))-policy::mul(u(0),v(2)),policy::mul(u(0),v(1))-policy::mul(u(1),v(0)));
	}


template<indexType _dim, class T, class policy>
inline
double fabs(const Vec<_dim, T, policy> &v){
	return v.abs();
	}

typedef Vec<2,double> vec2d;
typedef Vec<2,int> ivec2d;
typedef Vec<3,double> vec;
typedef Vec<3,double> vec3d;
typedef Vec<3,int> ivec3d;
typedef Vec<4,double> vec4d;

ivec2d ifloor(const vec2d &v){
	return ivec2d((int)floor(v(0)), (int)floor(v(1)));
	}
vec2d floor(const vec2d &v){
	return vec2d((int)floor(v(0)), (int)floor(v(1)));
	}

class HomVec: public vec4d{
 	typedef HomVec ret;
	public:
        HomVec(const double &x0, const double &x1, const double &x2, const double &x3=1.0):vec4d(x0, x1, x2, x3){}
        HomVec(const vec4d &v):vec4d(v){}
        HomVec(const vec3d &v, double w):vec4d(v(0),v(1), v(2), w){}
        HomVec():vec4d(0,0,0,1){}
	vec get3d()const{
		return vec((*this)(0), (*this)(1), (*this)(2));
		}

	vec project()const{
		static double w;
		w=(*this)(3);
		ERROR(w==0,"The point is projected to infinity");
		return vec((*this)(0)/w, (*this)(1)/w, (*this)(2)/w);
		}
	HomVec project4d()const{
		static double w;
		w=(*this)(3);
		ERROR(w==0,"The point is projected to infinity");
		return HomVec((*this)(0)/w, (*this)(1)/w, (*this)(2)/w,1);
		}
	};

#endif

