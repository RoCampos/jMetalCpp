//  PAES_main.cpp
//
//  Author:
//       Cristian Zambrano <cristian_uteq@hotmail.com>
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
#include <Operator.h>
#include <Solution.h>
#include <EdgeSolutionType.h>
#include <MMRPMutation.h>
#include <ProblemFactory.h>
#include <iostream>
#include <paes.h>
#include <time.h>
#include <QualityIndicator.h>

void help () {

  cout << "Usage:" << endl;
	cout << "\tNSGA2_main <ProblemName> <Instance> <populationSize> <maxEvaluations> <frontfile>\n";
	cout << "\nOptions:" << endl;
	cout << "\tProblemName	MMRP(Multisource Multicast Routing Problem)\n";
	cout << "\tInstance	Brite instance of MMRP\n";
	cout << "\tPopulationSize	number of individuals of the populations.\n";
	cout << "\tMaxEvaluations	number of iterations of the algorithms. Need to be > 10.\n";
	cout << "\tFrontFile	archive with the real pareto front\n";
	cout << "\nExamples:" << endl;
	cout << "\tNSGAII_main MMRP b30_1.brite 10 1000 frontfile.txt" << endl;

}

int main(int argc, char ** argv) {
  
  clock_t t_ini, t_fin;
  Problem   * problem   ; // The problem to solve
  Algorithm * algorithm ; // The algorithm to use
  Operator  * mutation  ; // Mutation operator
  
  if (argc>=2) {
    problem = ProblemFactory::getProblem(argc, argv);
    // cout << "Selected problem: " << problem->getName() << endl;
  } else {
    // cout << "No problem selected." << endl;
    // cout << "Default problem will be used: Fonseca" << endl;
   help ();
  }
  
  QualityIndicator * indicators;
  
  algorithm = new paes(problem);
  
  // Algorithm parameters
  int archiveSizeValue = atoi(argv[1]);
  int *archiveSizePtr = &archiveSizeValue;
  int biSectionsValue = atoi(argv[2]);
  int *biSectionsPtr = &biSectionsValue;
  int maxEvaluationsValue = atoi(argv[3]);
  int *maxEvaluationsPtr = &maxEvaluationsValue;
  std::string frontarchive = argv[4];
  
  algorithm->setInputParameter("archiveSize",archiveSizePtr);
  algorithm->setInputParameter("biSections",biSectionsPtr);
  algorithm->setInputParameter("maxEvaluations",maxEvaluationsPtr);
  
  // Mutation (Real variables)
  map<string, void *> parameters; // Operator parameters
  parameters.clear();
  mutation = new MMRPMutation(parameters);
  algorithm->addOperator("mutation",mutation);
  
  //creating the qualityindicador
	indicators = new QualityIndicator(problem, frontarchive);
	// Add the indicator object to the algorithm
	algorithm->setInputParameter("indicators", indicators) ;

  t_ini = clock();
  SolutionSet * population = algorithm->execute();
  t_fin = clock();
  double secs = (double) (t_fin - t_ini);
  secs = secs / CLOCKS_PER_SEC;
  
  // Result messages
  // cout << "Total execution time: " << secs << "s" << endl;
  // cout << "Variables values have been writen to file VAR" << endl;
  // population->printVariablesToFile("VAR");
  // cout << "Objectives values have been writen to file FUN" << endl;
  population->printObjectivesToFile("FUN");
  cout << indicators->getHypervolume(population) << endl;
  
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
  
} // main




