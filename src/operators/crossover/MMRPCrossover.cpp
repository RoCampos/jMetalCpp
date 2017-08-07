#include <MMRPCrossover.h>

MMRPCrossover::MMRPCrossover(map<string, void *> parameters)
: Crossover (parameters)
{



}

void * MMRPCrossover::execute (void* object) {

	void ** parameters = (void **) object;

	Solution * solution1 = (Solution *) parameters[0];
	Solution ** solution2 = (Solution **) parameters[1];

	Solution * child;

	//doing crossover
	//TODO como obter a representação


	return child;

}