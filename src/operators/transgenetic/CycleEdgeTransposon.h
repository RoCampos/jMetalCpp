#ifndef __CYCLE_TRANSPOSON
#define __CYCLE_TRANSPOSON

#include <Transposon.h>
#include <individual.h>
#include <mmrp.h>


class CycleEdgeTransposon : public Transposon
{
public:
	CycleEdgeTransposon(map<string, void*> parameters);
	void * execute (void*);
	
};

#endif