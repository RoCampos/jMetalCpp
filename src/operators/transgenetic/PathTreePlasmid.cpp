#include <PathTreePlasmid.h>

PathTreePlasmid::PathTreePlasmid (std::map<string, void*> parameters)
:  Plasmid(parameters)
{

}

void * PathTreePlasmid::execute (void * objects)
{

	
	void ** parameters = (void **) objects;

	Solution * current = (Solution *) parameters[0];
	Individual ind = current->get_representation ();
	Individual * best = (Individual *) parameters[1];

	Solution * child = new Solution (current->getNumberOfObjectives ());

	MMRP * mmrp = (MMRP *) current->getProblem();
	std::vector<rca::Group*> groups = mmrp->get_groups();

	for (int i=0; i < ind.size (); i++) 
	{
		double l = PseudoRandom::randDouble (0, 1.0);
		if (l < 0.3) {
			ind.cromossoma[i] = best->cromossoma.at (i);
		}
	}

	eval (ind, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	child->set_representation (ind);
	child->setType (current->getProblem()->getSolutionType());
	child->setProblem (current->getProblem());

	return child;
	
	
}