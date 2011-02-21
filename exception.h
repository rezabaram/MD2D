//=======================================================================
//======  Class CException  =====================================
// By Reza M. Baram, 2010
//=======================================================================
#ifndef EXCEPTION_H
#define EXCEPTION_H 
#include<iostream>
#include<sstream>

class CException
	{
	std::string message;
	std::string file;
	std::string function;
	long line;
	CException(){};
	public:
	CException(const std::string &m="Unknown", const std::string &f="UnknownFile", const std::string &func="UnknownFunction", long i=0):message(m), file(f), function(func), line(i) {};
	std::string where(){
		std::ostringstream out;
		out<<file<<":"<<line<<": "<<function<<": "<< message <<std::endl;
		return out.str();
		}
	void Report(){
		std::cerr<<"Error: "<<file<<":"<<line<<": "<<function<<": "<< message <<std::endl;
		}
	};

//some macros defined for convenience 
#define ERROR(b, m)  if(b){throw CException(m, __FILE__, __PRETTY_FUNCTION__, __LINE__);}
#define WARNING(x)  std::cerr<<"Warning: "<<__FILE__<<":"<<__LINE__<<": "<<x<<std::endl;
#define RETHROW(e)  throw CException((std::string)" \n\tfrom "+e.where(), __FILE__, __PRETTY_FUNCTION__, __LINE__);
#define TRY try{
#define CATCH  }\
	catch(CException e){ \
		RETHROW(e);\
		}\
	catch(...){\
		ERROR(1, "Unknown");\
		}\


#define CHECKPOINT throw CException("Checkpoint reached.", __FILE__, __PRETTY_FUNCTION__, __LINE__);
#endif /* EXCEPTION_H */
