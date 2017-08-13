#ifndef _MUTATION__
#define _MUTATION__

#include <Mutation.h>
#include <individual.h>
#include <mmrp.h>

class MMRPMutation : public Mutation
{
public:
	MMRPMutation(map<string, void *> parameters);
	virtual ~MMRPMutation();

	void * execute (void *);
	
};

#endif


