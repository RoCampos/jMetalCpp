#include <iostream>
#include <Algorithm.h>
#include <Problem.h>
#include <ProblemFactory.h>
#include <mota.h>
#include <Plasmid.h>
#include <PathPlasmid.h>
#include <Transposon.h>
#include <CycleEdgeTransposon.h>
#include <RandomSelection.h>
#include <SolutionSet.h>

#include <cstdlib>

void help () 
{

}

int main(int argc, char **argv)
{

	clock_t t_ini, t_fin;
	
	Problem   * problem   ; // The problem to solve
	Algorithm * algorithm ; // The algorithm to use

	if (argc >= 2) {
		problem = ProblemFactory::getProblem(argc, argv);
	} else {
		help ();
	}

	// algorithm`s parameters
	int populationSize = atoi(argv[1]);
	int maxEvaluations = atoi (argv[2]);
 	int hostInformationSize = atoi(argv[3]);
 	int elitePopSize = atoi (argv[4]);
 	std::string frontarchive = argv[5];
	std::string nadir = argv[6];

	//instance of the algorthm
	algorithm = new Mota(problem);

	std::map<std::string, void*> parameters;
	Operator * plasmid = new PathPlasmid (parameters);
	parameters.clear ();
	Operator * traspon = new CycleEdgeTransposon (parameters);

	parameters.clear ();
  	Operator * selection = new RandomSelection(parameters);

	//setting parameters 
	algorithm->setInputParameter("populationSize",&populationSize);
  	algorithm->setInputParameter("hostInformationSize",&hostInformationSize);
  	algorithm->setInputParameter("elitePopSize",&elitePopSize);
  	algorithm->setInputParameter("maxEvaluations",&maxEvaluations);

  	MMRP * mmrp = (MMRP *) problem;
  	rca::Network copy = *mmrp->get_network ();
  	for (int i = 0; i < copy.getNumberNodes (); ++i)
	{
		for (int j = 0; j < copy.getNumberNodes (); ++j)
		{	
			if (copy.getCost (i,j) > 0) {
				copy.setCost (i,j, 1);
				copy.setCost (j,i, 1);
			}
		}
	}

	algorithm->setInputParameter("networkCopy",&copy);

  	algorithm->addOperator ("PathPlasmid", plasmid);
  	algorithm->addOperator ("selection", selection);
  	algorithm->addOperator ("Transposon", traspon);

	t_ini = clock();
	SolutionSet * population = algorithm->execute();
	t_fin = clock();
	double secs = (double) (t_fin - t_ini);
	secs = secs / CLOCKS_PER_SEC;

	// cout << population->size () << endl;

	// cout << "Total execution time: " << secs << "s" << endl;
	population->printObjectivesToFile("FUN");

	std::ifstream nadirf;
	nadirf.open (nadir.c_str ());
	int Z, C, H;
	nadirf >> Z;
	nadirf >> C;
	nadirf >> H;
	nadirf.close ();
	std::stringstream ss;
	ss << "./hv FUN -r ";
	ss << '"' << Z << " " << C << " " << H <<'"';
	system (ss.str ().c_str());


	return 0;
}