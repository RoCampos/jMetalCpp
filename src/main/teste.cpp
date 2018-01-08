#include <iostream>

#include <Problem.h>
#include <Solution.h>
#include <ProblemFactory.h>
#include <MMRPCrossover.h>
#include <MMRPMutation.h>
#include <reader.h>

int main(int argc, char *argv[])
{
	
	Problem * problem;
	problem = ProblemFactory::getProblem(argc, argv);

	Solution * curr = new Solution(problem);
	problem->evaluate(curr);

	Solution * sol1 = new Solution(problem);
	problem->evaluate(sol1);
	Solution * sol2 = new Solution(problem);
	problem->evaluate(sol2);
	Solution * sol3 = new Solution(problem);
	problem->evaluate(sol3);

	Solution ** parents = new Solution*[3];
	parents[0] = sol1;
	parents[1] = sol2;
	parents[2] = sol3;

	MMRP * mmrp = (MMRP *) parents[0]->getProblem();
	int NODES = mmrp->get_network ()->getNumberNodes ();

	Individual & ind1 = sol1->get_representation ();
	Individual & ind2 = sol2->get_representation ();
	Individual & ind3 = sol3->get_representation ();

	ind1.str ();
	ind2.str ();
	ind3.str ();
	
	Individual novo (ind1.size ());
	diff_cross (ind1, ind2, ind3, novo, NODES);
	evaluate (novo, *mmrp->get_network (), mmrp->get_groups());

	novo.str ();
	novo.print ();


	// //has bugs 
	// void ** pointer = new void*[2];
	// pointer[0] = curr;
	// pointer[1] = parents;

	// std::string name = "THREE";
	// map<string, void *> parameters;
	// parameters["algorithm"] = &name;
	// Operator * crossover;
	// crossover = new MMRPCrossover(parameters);
	// Solution ** res = (Solution**) crossover->execute (parents);

	return 0;
}