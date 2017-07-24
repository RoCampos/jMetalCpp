
#ifndef _EDGE_SOLUTION_TYPE__
#define _EDGE_SOLUTION_TYPE__

#include <Problem.h>
#include <SolutionType.h>

//TODO includes my representation

class EdgeSolutionType : public SolutionType
{
public:
	EdgeSolutionType(Problem* problem);
	~EdgeSolutionType();

	Variable ** createVariables ();
	
};

#endif