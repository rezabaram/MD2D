/*By: 	Reza Baram
CopyRight: 
	Do whatever you want with this code. You can even replace 
	my name with yours. 

Disclaimer:
	There is no guarantee whatsoever that this code will work 
	perfectly as it is intended to. So, use is at your own
	risk.
*/


#ifndef CCONFIG_H
#define CCONFIG_H
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>

using namespace std;
class CParamBase {
	public:
		typedef enum {Normal, Default} out_type;
		
		CParamBase(string _name):name(_name){}
		virtual ~CParamBase();
		const string name;
		virtual void parse(istream &in)=0;
		virtual void print(ostream &out, CParamBase::out_type def=Normal)=0;
	
 	private:
	};


template <class T>
class CParam : public CParamBase {
	public:
		CParam(string _name, T _value):CParamBase(_name), value(_value), def_value(_value){};
		T get() const {
			return value;
			};
		T get_default() const {
			return def_value;
			};
		void set(T _value){
			value=_value;
			}
		void parse(istream &in){
			in>>value;//param.name<<":   "<<param.value;
			}
		void print(ostream &out, CParamBase::out_type def=Normal){
			if(def==CParamBase::Default) out<<def_value;

			else out<<value;
			}
		template <class U>
		friend istream &operator>>(istream &in, CParam<U> &param);

		template <class U>
		friend ostream &operator<<(ostream &out, CParam<U> &param);
 	private:
		CParam(){};
		T value;
		const T def_value;
	};

template <class T>
istream &operator>>(istream &in, CParam<T> &param){
			in>>param.value;//param.name<<":   "<<param.value;
			return in;
			}
template <class T>
ostream &operator<<(ostream &out, CParam<T> &param){
			out<<param.name<<":   "<<param.value;
			return out;
			}

// the class CBaseConfig is a singleton 
class CBaseConfig {
	public:
	CBaseConfig(){
		}; 
	virtual ~CBaseConfig();                                 
	//static CBaseConfig& Instance();
	bool isValid(string fname)const;
	void parse(string fname);
	void parse(istream &input);
	void print(string fname="config");
	void print(ostream &out=cout);
	virtual void define_parameters()=0;

	template<class T>
	T get_param(string name, CParamBase::out_type def=CParamBase::Normal)const {
		//const CParamBase  *pp=params[name];
	      if(!isValid(name)){
			cerr<< "Warning: "<<name<<" is not a valid parameter or keyword" <<endl;
			return T();
			}
		CParam<T> * const p = static_cast< CParam<T>* > (params.find(name)->second );//i dont know if there is a better solution, i like this tough

		if(def==CParamBase::Default) return p->get_default();
		else return p->get();
		};

	template<class T>
	void add_param(string name, T value){
		try{
		CParam<T> *p=new CParam<T>(name,value);
		params[name]=p;
		name_dict.push_back(name);
		}catch(...){
			cerr<< "error: invalid paramter." <<endl;
			}
		};

	map<string, CParamBase *> params;

	protected:
	vector<string> name_dict;

	CBaseConfig(const CBaseConfig&);                 // Prevent copy-construction

	private:
	static CBaseConfig *pConfig;
	CBaseConfig& operator=(const CBaseConfig&);      // Prevent assignment
};

#define  paramsDouble config.get_param<double> // just some abbreviations 
#define  paramsStr config.get_param<string> 

//---------------------------- definitions ------------------------------------------
CParamBase::~CParamBase(){}
CBaseConfig::~CBaseConfig(){
	map<string, CParamBase *>::iterator it;
	for(it=params.begin(); it!=params.end(); ++it) delete (*it).second;
	}                                 


/*
CBaseConfig * CBaseConfig::pConfig=NULL;
CBaseConfig &config=CBaseConfig::Instance();
CBaseConfig& CBaseConfig::Instance() {
	static CBaseConfig config;
	return config;
	}
*/

void CBaseConfig::print(string outname){
	ofstream outputFile(outname.c_str());
	if(!outputFile.good() ){
		cerr << "WARNING: Unable to open input file: " << outname << endl;
		return;
		}
	print(outputFile);
}

void CBaseConfig::print(ostream &out){
	map<string, CParamBase *>::iterator it;
	for(it=params.begin(); it != params.end(); it++){
		out<<it->first<<"  ";
		it->second->print(out);
		out<<endl;
		}
}

void CBaseConfig::parse(string infilename) {

	ifstream inputFile(infilename.c_str());

	if(!inputFile.good())
	{
	cerr << "WARNING: Unable to open input file: " << infilename << endl;
	return;
	}
	parse(inputFile);
	inputFile.close();
}

void CBaseConfig::parse(istream &inputFile) {
	const string comm="#";
	string line;
	string vname;

	//Parse the line
	while(getline(inputFile,line))
	{

	line = line.substr( 0, line.find(comm) );

	//Insert the line string into a stream
	stringstream ss(line);

	//Read up to first whitespace
	ss >> vname;

	//Skip to next line if this one starts with a # (i.e. a comment)
	if(vname.find("#",0)==0) continue;

	if(!isValid(vname)){
		cerr<< "Warning: "<<vname<<" is not a valid parameter or keyword" <<endl;
		continue;
		}

	//Read up to second whitespace
	params[vname]->parse(ss);
	}
}

bool CBaseConfig::isValid(string vname) const{
	vector<string>::const_iterator it;
	for( it=name_dict.begin(); it!=name_dict.end(); ++it){
		if(vname==(*it))return true;
		}
	return false;
	}


#endif
