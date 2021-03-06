#ifndef MMRP_CROSSOVER__
#define MMRP_CROSSOVER__


#include <Crossover.h>
#include <Solution.h>
#include <individual.h>
#include <EdgeSolutionType.h>
#include <myrandom.h>

class MMRPCrossover : public Crossover
{
public:
	MMRPCrossover(map<string, void *> parameters);
	void *execute (void *);

	
private:

	void * moead (void *);
	void * nsga (void *);

	rca::myrandom<std::mt19937, std::uniform_real_distribution<double>, double> generator_;

	int get_crossover ();
	
};


#endif