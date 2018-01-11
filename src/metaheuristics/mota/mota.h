#ifndef MOTA_H__
#define MOTA_H__

#include <Algorithm.h>
#include <Problem.h>
#include <SolutionSet.h>
#include <mmrp.h>
#include <genetichost.h>
#include <Distance.h>
#include <Ranking.h>
#include <CrowdingComparator.h>

class Mota : public Algorithm
{

public:
	Mota(Problem *);
	SolutionSet * execute (); 

private:
	void ns (
		SolutionSet * population, 
		Ranking * rank, 
		int populationSize, 
		Distance * distance);


};


#endif