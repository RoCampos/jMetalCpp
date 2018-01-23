
#include <MMRPMutation.h>


MMRPMutation::MMRPMutation (map<string, void *> parameters) 
: Mutation (parameters)
{
}

MMRPMutation::~MMRPMutation() {}


void * MMRPMutation::execute (void * object)
{
	
	Solution * sol = (Solution *) object;
	Individual & ind = sol->get_representation ();

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

}