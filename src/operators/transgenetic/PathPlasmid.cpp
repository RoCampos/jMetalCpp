#include <PathPlasmid.h>

PathPlasmid::PathPlasmid (std::map<string, void*> parameters)
: Plasmid (parameters)
{

}

void * PathPlasmid::execute (void * object) 
{

	void ** parameters = (void **) object;

	Solution * current = (Solution*) parameters[0];
	GeneticHost * gs = (GeneticHost*) parameters[1];

	MMRP * mmrp = (MMRP *) current->getProblem();
	std::vector<rca::Group*> groups = mmrp->get_groups();
	int GROUPS = groups.size ();

	Individual ind1 = current->get_representation();

	//nova solução
	Solution * child = new Solution (current->getNumberOfObjectives ());

	for (int i = 0; i < GROUPS; ++i)
	{
		//pegar uma gtree e substituir por um path
		std::vector<int> const & members = groups[i]->getMembers ();
		
		//destination (position)
		int m = rand () % members.size ();

		rca::Path path = gs->getRandomMeme (i, m);
		//simple replacement
		if (path.size () > 0)
			ind1.cromossoma.at (i).paths.at (m) = path;

	}

	evaluate (ind1, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	child->set_representation (ind1);
	child->setType (current->getProblem()->getSolutionType());
	child->setProblem (current->getProblem());

	return child;

}