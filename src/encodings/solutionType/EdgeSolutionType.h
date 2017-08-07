
#ifndef _EDGE_SOLUTION_TYPE__
#define _EDGE_SOLUTION_TYPE__

#include <Problem.h>
#include <SolutionType.h>
#include <myoperator.h>
#include <mmrp.h>
#include <individual.h>


//TODO includes my representation

class EdgeSolutionType : public SolutionType
{
public:
	EdgeSolutionType(Problem* problem);
	Variable ** createVariables ();

private:

	Individual individual;
	WidestPathTree stp1;
	ShortestPathTree stp2;
	BestFirstSearch stp3;
	
};

#endif