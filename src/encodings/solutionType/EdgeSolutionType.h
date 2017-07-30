
#ifndef _EDGE_SOLUTION_TYPE__
#define _EDGE_SOLUTION_TYPE__

#include <Problem.h>
#include <SolutionType.h>

#include <individual.h>

//TODO includes my representation

class EdgeSolutionType : public SolutionType
{
public:
	EdgeSolutionType(Problem* problem);
	Variable ** createVariables ();
	
};

#endif