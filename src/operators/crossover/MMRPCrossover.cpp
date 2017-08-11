#include <MMRPCrossover.h>

MMRPCrossover::MMRPCrossover(map<string, void *> parameters)
: Crossover (parameters)
{



}

void * MMRPCrossover::execute (void* object) {

	std::cout << __FUNCTION__ << std::endl;

	void ** parameters = (void **) object;

	Solution * current = (Solution *) parameters[0];
	Solution ** parent = (Solution **) parameters[1];

	MMRP * mmrp = (MMRP *) current->getProblem();

	Solution * child = new Solution (current->getNumberOfObjectives ());

	Individual &ind1 = parent[0]->get_representation();
	Individual &ind2 = parent[1]->get_representation();
	Individual novo; 

	int CSIZE = ind1.cromossoma.size ();
	novo.cromossoma = std::vector<DTree> (CSIZE);
	novo.objectives = std::vector<double> (3, 0);
	novo.cost = std::vector<int> (CSIZE);

	//Applying crossover on the solution
	crossover_by_hop (ind1, ind2, novo, 30);

	evaluate (novo, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	child->set_representation (novo);
	child->setType (current->getProblem()->getSolutionType());

	return child;

}