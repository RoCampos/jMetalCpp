//  MOEAD_main.cpp
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
#include <ProblemFactory.h>
#include <MOEAD.h>
#include <PolynomialMutation.h>
#include <MMRPCrossover.h>
#include <MMRPMutation.h>
#include <iostream>
#include <time.h>
#include <QualityIndicator.h>

/**
 * This class executes the algorithm described in:
 *   H. Li and Q. Zhang, 
 *   "Multiobjective Optimization Problems with Complicated Pareto Sets,  MOEA/D 
 *   and NSGA-II". IEEE Trans on Evolutionary Computation, vol. 12,  no 2,  
 *   pp 284-302, April/2009.  
 */

void help () {

}

int main(int argc, char ** argv) {

  clock_t t_ini, t_fin;

  Problem   * problem;   // The problem to solve
  Algorithm * algorithm; // The algorithm to use
  Operator  * crossover; // Crossover operator
  Operator  * mutation;  // Mutation operator
  
  QualityIndicator * indicators ; // Object to get quality indicators

  map<string, void *> parameters; // Operator parameters

  //TODO: QualityIndicator * indicators; // Object to get quality indicator

  if (argc>=2) {
    problem = ProblemFactory::getProblem(argc, argv);
    // cout << "Selected problem: " << problem->getName() << endl;
  } else {
    // cout << "No problem selected." << endl;
    // cout << "Default problem will be used: Kursawe" << endl;
    help ();
  }

  algorithm = new MOEAD(problem);

  int populationSizeValue = atoi (argv[1]);
  int maxEvaluationsValue = atoi (argv[2]);
  std::string frontarchive = argv[3];
  std::string nadir = argv[4];

  algorithm->setInputParameter("populationSize",&populationSizeValue);
  algorithm->setInputParameter("maxEvaluations",&maxEvaluationsValue);
  
  // Directory with the files containing the weight vectors used in 
  // Q. Zhang,  W. Liu,  and H Li, The Performance of a New Version of MOEA/D 
  // on CEC09 Unconstrained MOP Test Instances Working Report CES-491, School 
  // of CS & EE, University of Essex, 02/2009.
  // http://dces.essex.ac.uk/staff/qzhang/MOEAcompetition/CEC09final/code/ZhangMOEADcode/moead0305.rar
  string dataDirectoryValue =
          "./data/Weight";
  algorithm->setInputParameter("dataDirectory", &dataDirectoryValue);

  
  std::string name = "ONE";
  double c1 = 0.25;
  parameters["algorithm"] = &name;
  parameters["crossover1"] = &c1;
  parameters["crossover2"] = &c1;
  parameters["crossover3"] = &c1;
  parameters["crossover4"] = &c1;
  crossover = new MMRPCrossover (parameters);
  
  // Mutation operator
  parameters.clear();
  mutation = new MMRPMutation(parameters);

  // Add the operators to the algorithm
  algorithm->addOperator("crossover",crossover);
  algorithm->addOperator("mutation",mutation);

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
  cout << "Total execution time: " << secs << "s" << endl;
  // cout << "Variables values have been written to file VAR" << endl;
  // population->printVariablesToFile("VAR");
  // cout << "Objectives values have been written to file FUN" << endl;
  population->printObjectivesToFile("MOEADRES", true);
  // cout << indicators->getHypervolume(population) << endl;
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


  delete mutation;
  delete crossover;
  delete population;
  delete algorithm;

} // main

