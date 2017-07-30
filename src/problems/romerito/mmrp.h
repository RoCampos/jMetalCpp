
/**

	Class wrapper to incude my source code into jMetal Framework

*/

#ifndef _MMRP__
#define _MMRP__

#include <Problem.h>
#include <Solution.h>
#include <EdgeSolutionType.h>
#include <network.h>

class MMRP : public Problem
{

public:
	
	MMRP(std::string);
	MMRP(std::string solutionType, std::string instance);
	~MMRP();
	void evaluate (Solution *solution);
	
};

#endif