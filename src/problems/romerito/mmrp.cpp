#include <mmrp.h>


MMRP::MMRP (std::string solutionType)
{
	//solution type do not reprensent the variables
	numberOfVariables_ = 0;

	//the problem has tree objectives
	numberOfObjectives_ = 3;

	//there is no constraints
	numberOfConstraints_ = 0;

	//multi-source multicast routing problem
	problemName_ = "MMRP";

	//defined in Problem.h
	//It uses my representation (wrapper)
	solutionType_ = new EdgeSolutionType (this);

}

void MMRP::evaluate (Solution * solution) 
{

}