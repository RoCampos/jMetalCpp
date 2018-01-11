#ifndef PLASMID_H_
#define PLASMID_H_ 

#include <Operator.h>

class Plasmid : public Operator
{
public:
	Plasmid(std::map<string, void*> parameters);
	virtual ~Plasmid() = 0;
	
};

#endif