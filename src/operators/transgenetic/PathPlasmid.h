#ifndef __PATH_PLASMID_H__
#define __PATH_PLASMID_H__

#include <Plasmid.h>
#include <string>
#include <map>
#include <Solution.h>
#include <GeneticHost.h>
#include <group.h>
#include <mmrp.h>

class PathPlasmid : public Plasmid
{

public:
	PathPlasmid(std::map<string, void*> parameters);
	void * execute (void*);
	
};

#endif

