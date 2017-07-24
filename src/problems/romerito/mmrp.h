
/**

	Class wrapper to incude my source code into jMetal Framework

*/

#ifndef _MMRP_
#define _MMRP_

#include <Problem.h>
#include <Solution.h>
#include <EdgeSolutionType.h>

class MMRP : public Problem
{

public:
	
	MMRP(std::string solutionType);
	~MMRP();
	void evaluate (Solution *solution);
	
};

#endif