//  GDE3_main.cpp
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
#include <Algorithm.h>
#include <Solution.h>
#include <Operator.h>
#include <QualityIndicator.h>
#include <GDE3.h>
#include <MMRPCrossover.h>
#include <DifferentialEvolutionSelection.h>
#include <ProblemFactory.h>
#include <iostream>
#include <time.h>

void help () {

  cout << "Usage:" << endl;
	cout << "\tGDE3_main <ProblemName> <Instance> <populationSize> <maxEvaluations> <frontfile>\n";
	cout << "\nOptions:" << endl;
	cout << "\tProblemName	MMRP(Multisource Multicast Routing Problem)\n";
	cout << "\tInstance	Brite instance of MMRP\n";
	cout << "\tPopulationSize	number of individuals of the populations.\n";
	cout << "\tMaxEvaluations	number of iterations of the algorithms. Need to be > 10.\n";
	cout << "\tFrontFile	archive with the real pareto front\n";
  cout << "\tNadirFile  archive with the worst Objective vector v;\n";
  cout << "\tbuilder is the parameter to control the tree contruction\n";
	cout << "\nExamples:" << endl;
	cout << "\tGDE3_main MMRP b30_1.brite 10 1000 frontfile.txt nadir.txt builder" << endl;

	exit (0);

}

int main(int argc, char ** argv) {

  srand (time(NULL));

  clock_t t_ini, t_fin;

  Problem   * problem   ; // The problem to solve
  Algorithm * algorithm ; // The algorithm to use
  Operator  * crossover ; // Crossover operator
  Operator  * selection ; // Selection operator

  map<string, void *> parameters;
  
  QualityIndicator * indicators;
  indicators = NULL;

  if (argc>=2) {
    problem = ProblemFactory::getProblem(argc, argv);
    // cout << "Selected problem: " << problem->getName() << endl;
  } else {
    help ();
  }

  algorithm = new GDE3(problem);

  // Algorithm parameters
  int populationSizeValue = atoi(argv[1]);
  int maxIterationsValue = atoi(argv[2]);
  std::string frontarchive = argv[3];
  std::string nadir = argv[4];
  int builder = atoi (argv[5]);
  algorithm->setInputParameter("populationSize",&populationSizeValue);
  algorithm->setInputParameter("maxIterations",&maxIterationsValue);
  algorithm->setInputParameter("builder", &builder);

  // Crossover operator
  std::string name = "THREE";
  parameters["algorithm"] = &name;
  crossover = new MMRPCrossover(parameters);

  // Selection operator
  parameters.clear();
  selection = new DifferentialEvolutionSelection(parameters) ;

  // Add the operators to the algorithm
  algorithm->addOperator("crossover",crossover);
  algorithm->addOperator("selection",selection);

  // indicators = new QualityIndicator(problem, frontarchive);
  // Add the indicator object to the algorithm
  // algorithm->setInputParameter("indicators", indicators) ;

  // Execute the Algorithm
  t_ini = clock();
  SolutionSet * population = algorithm->execute();
  t_fin = clock();
  double secs = (double) (t_fin - t_ini);
  secs = secs / CLOCKS_PER_SEC;

  // Result messages
  cout << "Total execution time: " << secs << "s" << endl;
  
  // cout << "Variables values have been written to file VAR" << endl;
  // population->printVariablesToFile("VAR");
  // cout << "Objectives values have been written to file FUN" << endl;
  population->printObjectivesToFile("gde");
  // cout << indicators->getHypervolume(population) << endl;

  // std::ifstream nadirf;
  // nadirf.open (nadir.c_str ());
  // int Z, C, H;
  // nadirf >> Z;
  // nadirf >> C;
  // nadirf >> H;
  // nadirf.close ();
  // std::stringstream ss;
  // ss << "./hv FUN -r ";
  // ss << '"' << Z << " " << C << " " << H <<'"';
  // system (ss.str ().c_str());

  delete selection;
  delete crossover;
  delete population;
  delete algorithm;

} // main
