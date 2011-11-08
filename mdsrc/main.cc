#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
#include <cctype>
#include "exception.h"
#include "MersenneTwister.h"
#include "mdsys.h"

long RNGSeed;
extern MTRand rgen;
int main(int pi, char **params){
	if(pi==1)
		RNGSeed=0;
	else
		RNGSeed=313*atoi(params[1])+1;

	//cerr<<"RNG Seed: "<<RNGSeed<<endl;
	
	try {
	Initialize();

	Run();

	Shutdown();
	exit(0);
	} catch(CException e)
	{
	e.Report();
	exit(1);
	}catch(...){
		cerr<< "Unknow error occure." <<endl;
		}

}


