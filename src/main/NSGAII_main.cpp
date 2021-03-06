//  NSGAII_main.cpp
//
//  Author:
//       Esteban López-Camacho <esteban@lcc.uma.es>
//
//  Copyright (c) 2011 Antonio J. Nebro, Juan J. Durillo
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <Problem.h>
#include <Solution.h>
#include <SBXCrossover.h>
#include <PolynomialMutation.h>
#include <BinaryTournament2.h>
#include <iostream>
#include <NSGAII.h>
#include <ProblemFactory.h>
#include <string.h>
#include <time.h>
#include <MMRPCrossover.h>
#include <MMRPMutation.h>

#include <cstdlib>


/**
 * Class implementing the NSGA-II algorithm.
 * This implementation of NSGA-II makes use of a QualityIndicator object
 *  to obtained the convergence speed of the algorithm. This version is used
 *  in the paper:
 *     A.J. Nebro, J.J. Durillo, C.A. Coello Coello, F. Luna, E. Alba
 *     "A Study of Convergence Speed in Multi-Objective Metaheuristics."
 *     To be presented in: PPSN'08. Dortmund. September 2008.
 *
 *   Besides the classic NSGA-II, a steady-state version (ssNSGAII) is also
 *   included (See: J.J. Durillo, A.J. Nebro, F. Luna and E. Alba
 *                  "On the Effect of the Steady-State Selection Scheme in
 *                  Multi-Objective Genetic Algorithms"
 *                  5th International Conference, EMO 2009, pp: 183-197.
 *                  April 2009)
 */


void help () {

	cout << "Usage:" << endl;
	cout << "\tNSGA2_main <ProblemName> <Instance> <populationSize> <maxEvaluations> <frontfile> <nadir>\n";
	cout << "\nOptions:" << endl;
	cout << "\tProblemName	MMRP(Multisource Multicast Routing Problem)\n";
	cout << "\tInstance	Brite instance of MMRP\n";
	cout << "\tPopulationSize	number of individuals of the populations.\n";
	cout << "\tMaxEvaluations	number of iterations of the algorithms. Need to be > 10.\n";
	cout << "\tFrontFile	archive with the real pareto front\n";
	cout << "\tNadir worst point in the search space\n";
	cout << "\nExamples:" << endl;
	cout << "\tNSGAII_main MMRP b30_1.brite 10 1000 frontfile.txt nadir.txt" << endl;


	exit (0);
}

int main(int argc, char ** argv) {

	clock_t t_ini, t_fin;
  
  	Problem   * problem   ; // The problem to solve
	Algorithm * algorithm ; // The algorithm to use
	Operator  * crossover ; // Crossover operator
	Operator  * mutation  ; // Mutation operator
	Operator  * selection ; // Selection operator

	if (argc>=2) {
		problem = ProblemFactory::getProblem(argc, argv);
		// cout << "Selected problem: " << problem->getName() << endl;
	} else {
		// cout << "No problem selected." << endl;
		// cout << "Default problem will be used: Fonseca" << endl;
		help ();
	}
  
 	QualityIndicator * indicators ; // Object to get quality indicators
	indicators = NULL ;

	algorithm = new NSGAII(problem);

  	// Algorithm parameters
 	int populationSize = atoi(argv[1]);
 	int maxEvaluations = atoi(argv[2]);
	std::string frontarchive = argv[3];
	std::string nadir = argv[4];

 	algorithm->setInputParameter("populationSize",&populationSize);
  	algorithm->setInputParameter("maxEvaluations",&maxEvaluations);

	// Mutation and Crossover for Real codification
  	std::string name = "TWO";
	map<string, void *> parameters;
	parameters["algorithm"] = &name;
  	crossover = new MMRPCrossover(parameters);

	parameters.clear();
 	mutation = new MMRPMutation(parameters);

	// Selection Operator
	parameters.clear();
	selection = new BinaryTournament2(parameters);

	// Add the operators to the algorithm
	algorithm->addOperator("crossover",crossover);
	algorithm->addOperator("mutation",mutation);
	algorithm->addOperator("selection",selection);


	//creating the qualityindicador
	indicators = new QualityIndicator(problem, frontarchive);
	// Add the indicator object to the algorithm
	algorithm->setInputParameter("indicators", indicators) ;

	// MMRP * p = (MMRP*) problem;
	// Network * n = p->get_network ();
	// for (auto v : n->get_neighboors (0)) {
	// 	cout << v << endl;
	// }

	// exit (1);


	// Execute the Algorithm
	t_ini = clock();
	SolutionSet * population = algorithm->execute();
	t_fin = clock();
	double secs = (double) (t_fin - t_ini);
	secs = secs / CLOCKS_PER_SEC;

	// Result messages
	// cout << "Total execution time: " << secs << "s" << endl;
	// cout << "Variables values have been written to file VAR" << endl;
	// population->printVariablesToFile("VAR");
	// cout << "Objectives values have been written to file FUN" << endl;
	population->printObjectivesToFile("FUN");
	//cout << indicators->getHypervolume(population) << endl;
  
//  if (indicators != NULL) {
//    cout << "Quality indicators" << endl;
//    cout << "Hypervolume: " << indicators->getHypervolume(population) << endl;
//    cout << "GD         : " << indicators->getGD(population) << endl;
//    cout << "IGD        : " << indicators->getIGD(population) << endl;
//    cout << "Spread     : " << indicators->getSpread(population) << endl;
//    cout << "Epsilon    : " << indicators->getEpsilon(population) << endl;
//    
//    int evaluations = *(int *) algorithm->getOutputParameter("evaluations");;
//    cout << "Speed      : " << evaluations << " evaluations" << endl;
//  } // if

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
	

  delete selection;
  delete mutation;
  delete crossover;
  delete population;
  delete algorithm;

} // main
