#ifndef __PATH_TREE_PLASMID
#define __PATH_TREE_PLASMID 

#include <Plasmid.h>
#include <PseudoRandom.h>
#include <group.h>
#include <mmrp.h>

class PathTreePlasmid : public Plasmid
{
public:
	PathTreePlasmid(std::map<string, void*> parameters);
	void * execute (void *);
	
};

#endif