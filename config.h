#ifndef CONFIG_H
#define CONFIG_H 
#include"baseconfig.h"
#include"vec.h"

class CConfig : public CBaseConfig{
	public:
	CConfig(string filename){
		define_parameters();
		parse(filename);
		}
	void define_parameters(){
		add_param<double>("Lx",1.0);
		add_param<double>("Ly",2.0);
		add_param<double>("size",0.02);
		add_param<bool>("periodic_x",true);
		add_param<bool>("periodic_y",false);
		add_param<double>("maxtime", 50);
		add_param<double>("outDt",0.1);
		add_param<double>("outStart",0.0);
		add_param<double>("outEnd",50);
		add_param<vec2d>("G",vec2d(0,0));
		add_param<double>("wall_pressure", 1);
		add_param<double>("r_var", 1);
		}
	};

#endif /* CONFIG_H */
