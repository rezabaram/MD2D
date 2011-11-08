#ifndef TOOLS_H
#define TOOLS_H 
#include<math.h>
#include<stdlib.h>
#include<sstream>
using namespace std;

double power_law(double x0, double x1, double n){
	double y=drand48();
	return pow((pow(x1,(n+1)) - pow(x0,(n+1)))*y + pow(x0,(n+1)), (1/(n+1.)) );
	}

bool isNumeric( const char* pszInput, int nNumberBase=10 )
{
	istringstream iss( pszInput );
 
	if ( nNumberBase == 10 )
	{
		double dTestSink;
		iss >> dTestSink;
	}
	else if ( nNumberBase == 8 || nNumberBase == 16 )
	{
		int nTestSink;
		iss >> ( ( nNumberBase == 8 ) ? oct : hex ) >> nTestSink;
	}
	else
		return false;
 
	// was any input successfully consumed/converted?
	if ( ! iss )
		return false;
 
	// was all the input successfully consumed/converted?
	return ( iss.rdbuf()->in_avail() == 0 );
}
#endif /* TOOLS_H */
