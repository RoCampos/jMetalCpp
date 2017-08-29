#include <MMRPCrossover.h>

MMRPCrossover::MMRPCrossover(map<string, void *> parameters)
: Crossover (parameters)
{

	generator_ = 
		rca::myrandom<std::mt19937, std::uniform_real_distribution<double>, double> 
		(time(NULL), 0.0, 1.0);

}

void * MMRPCrossover::execute (void* object) {

	std::string algo = *(std::string*) this->parameters_["algorithm"];

	if (algo.compare ("TWO") == 0) {
		return this->nsga (object);
	} else if (algo.compare ("ONE") == 0) {
		Solution * child = (Solution *)this->moead (object);
		return child;
	} else {
		std::cout << "You should check if the right crossover operator has been implemented\n";
		exit (1);
	}
}

void * MMRPCrossover::moead (void * object)
{
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


	double cross = this->generator_.rand ();

	if (cross < 0.25) {
		crossover_by_hop (ind1, ind2, novo, 30);	
	} else if (cross >= 0.25 && cross < 0.5) {
		crossover_by_tree (ind1, ind2, novo); 
	} else if (cross >= 0.5 && cross < 0.75) {
		crossover_by_path (ind1, ind2, novo, 30);
	} else {
		crossover_by_tcost (ind1, ind2, novo);
	}

	evaluate (novo, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	child->set_representation (novo);
	child->setType (current->getProblem()->getSolutionType());
	child->setProblem (current->getProblem());

	return child;
}

void* MMRPCrossover::nsga (void * object)
{

	Solution ** parents = (Solution**) object;
	MMRP * mmrp = (MMRP *) parents[0]->getProblem();
	int NODES = mmrp->get_network ()->getNumberNodes ();

	Solution ** childs = new Solution*[2];

	childs[0] = new Solution (parents[0]->getNumberOfObjectives ());
	childs[0]->setType (parents[0]->getProblem()->getSolutionType());
	childs[0]->setProblem (parents[0]->getProblem());

	childs[1] = new Solution (parents[0]->getNumberOfObjectives ());
	childs[1]->setType (parents[0]->getProblem()->getSolutionType());
	childs[1]->setProblem (parents[0]->getProblem());

	Individual &ind1 = parents[0]->get_representation();
	Individual &ind2 = parents[1]->get_representation();
	Individual novo1, novo2; 

	int CSIZE = ind1.cromossoma.size ();
	novo1.cromossoma = std::vector<DTree> (CSIZE);
	novo1.objectives = std::vector<double> (3, 0);
	novo1.cost = std::vector<int> (CSIZE);

	novo2.cromossoma = std::vector<DTree> (CSIZE);
	novo2.objectives = std::vector<double> (3, 0);
	novo2.cost = std::vector<int> (CSIZE);

	int op = this->get_crossover ();

	if (op == 1) {
		crossover_by_hop (ind1, ind2, novo1, 30);	
	} else if (op == 2) {
		crossover_by_tree (ind1, ind2, novo1); 
	} else if (op == 3) {
		crossover_by_path (ind1, ind2, novo1, 30);
	} else {
		crossover_by_tcost (ind1, ind2, novo1);
	}

	op = this->get_crossover ();

	if (op == 1) {
		crossover_by_hop (ind1, ind2, novo2, 30);	
	} else if (op == 2) {
		crossover_by_tree (ind1, ind2, novo2); 
	} else if (op == 3) {
		crossover_by_path (ind1, ind2, novo2, 30);
	} else {
		crossover_by_tcost (ind1, ind2, novo2);
	}

	evaluate (novo1, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	evaluate (novo2, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	childs[0]->set_representation (novo1);
	childs[1]->set_representation (novo2);


	return childs;
}

int MMRPCrossover::get_crossover ()
{

	double cross = this->generator_.rand ();

	if (cross < 0.25) {
		return (1);
	} else if (cross >= 0.25 && cross < 0.5) {
		return (2);
	} else if (cross >= 0.5 && cross < 0.75) {
		return (3);
	} else {
		return (4);
	}

}
