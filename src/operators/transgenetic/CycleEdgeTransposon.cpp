#include <CycleEdgeTransposon.h>

CycleEdgeTransposon::CycleEdgeTransposon (
	map<string, void*> parameters)
: Transposon (parameters)
{

}

void* CycleEdgeTransposon::execute (void * object) 
{

	Solution * sol = (Solution *) object;
	Individual ind = sol->get_representation ();

	Solution * child = new Solution (sol->getNumberOfObjectives ());

	MMRP * mmrp = (MMRP*) sol->getProblem ();

	std::vector<rca::Group> tmp;
	for (int i = 0; i < mmrp->get_groups().size (); ++i)
	{
		tmp.push_back (*mmrp->get_groups().at(i));
	}

	mutation (ind,mmrp->get_network (), tmp);

	eval (ind, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	child->set_representation (ind);
	child->setType (sol->getProblem()->getSolutionType());
	child->setProblem (sol->getProblem());

	return child;
	// return sol;

}
