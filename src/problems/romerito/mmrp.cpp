#include <mmrp.h>


MMRP::MMRP(std::string)
{
	//default constructor
}

MMRP::MMRP (std::string solutionType, std::string instance)
{
	//solution type do not reprensent the variables
	numberOfVariables_ = 0;

	//the problem hxas tree objectives
	numberOfObjectives_ = 3;

	//there is no constraints
	numberOfConstraints_ = 0;

	//multi-source multicast routing problem
	problemName_ = "MMRP";

	//defined in Problem.h
	//It uses my representation (wrapper)
	solutionType_ = new EdgeSolutionType (this);

}

MMRP::~MMRP()
{



}

void MMRP::evaluate (Solution * solution) 
{

}