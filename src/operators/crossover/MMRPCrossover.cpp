#include <MMRPCrossover.h>

MMRPCrossover::MMRPCrossover(map<string, void *> parameters)
: Crossover (parameters)
{



}

void * MMRPCrossover::execute (void* object) {

	void ** parameters = (void **) object;

	Solution * current = (Solution *) parameters[0];
	Solution ** parent = (Solution **) parameters[1];

	Solution * child = new Solution (current->getNumberOfObjectives ());

	Individual &ind1 = ((EdgeSolutionType*) parent[0]->getType ())->get_representation();
	Individual &ind2 = ((EdgeSolutionType*) parent[1]->getType ())->get_representation();
	Individual novo; 

	int CSIZE = ind1.cromossoma.size ();
	novo.cromossoma = std::vector<DTree> (CSIZE);
	novo.objectives = std::vector<double> (3, 0);
	novo.cost = std::vector<int> (CSIZE);

	//Applying crossover on the solution
	crossover_by_tree (ind1, ind2, novo);

	EdgeSolutionType * est = new EdgeSolutionType (current->getProblem ());
	est->set_representation (novo);
	child->setType (est);

	return child;

}