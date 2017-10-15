//  SMSEMOA_main.cpp
//
//  Author:
//        Cristian Zambrano V. <cristian_uteq@hotmail.com>
//
//  Copyright (c) 2013 Antonio J. Nebro, Juan J. Durillo
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
#include <MMRPCrossover.h>
#include <MMRPMutation.h>
#include <RandomSelection.h>
#include <iostream>
#include <SMSEMOA.h>
#include <ProblemFactory.h>
#include <string.h>
#include <time.h>
#include <QualityIndicator.h>



void help () {

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

  //TODO: Quality Indicators
  QualityIndicator * indicators ; // Object to get quality indicators
  indicators = NULL ;

  algorithm = new SMSEMOA(problem);

  // Algorithm parameters
  int populationSize = atoi(argv[1]);
  int maxEvaluations = atoi(argv[2]);
  double offset = atoi(argv[3]);
  std::string frontarchive = argv[4];
  std::string nadir = argv[5];

  algorithm->setInputParameter("populationSize",&populationSize);
  algorithm->setInputParameter("maxEvaluations",&maxEvaluations);
  algorithm->setInputParameter("offset",&offset);

  // Mutation
  map<string, void *> parameters; 
  std::string name = "TWO";
  parameters["algorithm"] = &name;
  crossover = new MMRPCrossover(parameters);

  //Crossover
  parameters.clear();
  
  mutation = new MMRPMutation(parameters);

  // Selection Operator
  parameters.clear();
  selection = new RandomSelection(parameters);

	// Add the operators to the algorithm
  algorithm->addOperator("crossover",crossover);
  algorithm->addOperator("mutation",mutation);
  algorithm->addOperator("selection",selection);

  //creating the qualityindicador
	indicators = new QualityIndicator(problem, frontarchive);
  // Add the indicator object to the algorithm
  algorithm->setInputParameter("indicators", indicators) ;

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
  population->printObjectivesToFile("SMSEMOARES", true);
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

	delete mutation;
	delete crossover;
	delete selection;
  delete population;
	delete algorithm;

} // main
