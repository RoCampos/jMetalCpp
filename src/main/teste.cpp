#include <iostream>
#include <Algorithm.h>
#include <Problem.h>
#include <ProblemFactory.h>
#include <mota.h>
#include <Plasmid.h>
#include <PathPlasmid.h>
#include <PathTreePlasmid.h>
#include <Transposon.h>
#include <CycleEdgeTransposon.h>
#include <RandomSelection.h>
#include <SolutionSet.h>

#include <cstdlib>

using namespace std;

int main(int argc, char *argv[])
{
	Problem   * problem   ; // The problem to solve
	Algorithm * algorithm ; // The algorithm to use
	if (argc >= 2) {
		problem = ProblemFactory::getProblem(argc, argv);
	} 

	SolutionSet * population = new SolutionSet(100);
	for (int i=0; i < 100; i++) {
		population->add (new Solution (problem));
	}

	MMRP * mmrp = (MMRP *) problem;
	Individual individual = Individual (mmrp->get_groups().size ());
	for (int i = 0; i < individual.size (); i++) {
		individual.cost[i] = std::numeric_limits<int>::max();
	}

	for (int i = 0; i < population->size (); i++) {
		Solution * sol = population->get(i);
		Individual & ind = sol->get_representation ();
		for (int j=0; j < ind.size (); j++) {	
			if (ind.cost[j] < individual.cost[j]){
				individual.cromossoma[j] = ind.cromossoma[j];
				individual.cost[j] = ind.cost[j];
			}
		}
	}


	std::map<std::string, void*> parameters;
	Operator *op = new PathTreePlasmid (parameters);

	void** objects = new void*[2];
	objects[0] = population->get(0);
	objects[1] = &individual;

	population->get(0)->get_representation().str ();
	Solution *sol = (Solution*) op->execute (objects);
	sol->get_representation ().str ();





	return 0;
}