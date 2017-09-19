
/**

	Class wrapper to incude my source code into jMetal Framework

*/

#ifndef _MMRP__
#define _MMRP__

#include <memory>

#include <Problem.h>
#include <Solution.h>
#include <EdgeSolutionType.h>
#include <network.h>
#include <group.h>
#include <reader.h>

class MMRP : public Problem
{

public:
	
	MMRP(std::string);
	MMRP(std::string solutionType, std::string instance);
	~MMRP();
	void evaluate (Solution *solution);

	rca::Network * get_network () {return network_;};
	rca::Group * get_group (int i) {return mgroups_.at (i);}
	size_t numberGroups () {return mgroups_.size ();}
	std::vector<rca::Group*> & get_groups () {return mgroups_;}

private:

	rca::Network * network_;
	std::vector<rca::Group*> mgroups_;
};

#endif