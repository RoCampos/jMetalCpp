#ifndef __Transposon_H__
#define __Transposon_H__

#include <Operator.h>

class Transposon : public Operator
{
public:
	Transposon();
	Transposon(map<string, void*> parameters);
	virtual ~Transposon();

	
};

#endif