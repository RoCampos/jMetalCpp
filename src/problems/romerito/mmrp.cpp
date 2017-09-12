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
	//Inicialização ocorre aqui
	solutionType_ = new EdgeSolutionType (this);

	//configurando rede e grupos
	rca::reader::MultipleMulticastReader reader(instance);
	network_ = new rca::Network;
	reader.readerGroup (mgroups_);
	reader.configNetwork (network_);

}

MMRP::~MMRP()
{
	for (int i = 0; i < mgroups_.size (); ++i)
	{
		delete mgroups_[i];
	}

}

void MMRP::evaluate (Solution * solution) 
{

	EdgeSolutionType * tmp = (EdgeSolutionType*) solution->getType ();
	
	solution->setObjective (0,tmp->get_representation().objectives.at (0));
	solution->setObjective (1,tmp->get_representation().objectives.at (1));
	solution->setObjective (2,tmp->get_representation().objectives.at (2));

}