
#include <mota.h>


Mota::Mota (Problem * problem) : Algorithm(problem)
{

}

SolutionSet * Mota::execute () {

	SolutionSet * subpop1;
	SolutionSet * subpop2;
	SolutionSet * subpop3;
	SolutionSet * elite;

	SolutionSet * offspring1;
	SolutionSet * offspring2;
	SolutionSet * offspring3;

	Operator * plasmid;
	Operator * selection;

	//for crownding distance
	Distance * distance = new Distance();

	//parameters of algorithms
	int populationSize = *(int*) getInputParameter ("populationSize");
	int hostInformationSize = *(int*) getInputParameter("hostInformationSize");
	int elitePopSize = *(int*) getInputParameter ("elitePopSize");
	int maxEvaluations = *(int *) getInputParameter("maxEvaluations");


	//creatig hostInformationSize paths for each pair s,d for all D
	MMRP * mmrp = (MMRP *) problem_;
	GeneticHost gs (hostInformationSize);
	gs.execute (mmrp->get_network (), mmrp->get_groups());

	//solutions sets 
	subpop1 = new SolutionSet ((int)populationSize/3);
	subpop2 = new SolutionSet ((int)populationSize/3);
	subpop3 = new SolutionSet ((int)populationSize/3);

	plasmid 	= operators_["PathPlasmid"];
	selection	= operators_["selection"];

	Solution * sol;
	for (int i = 0; i < populationSize/3; ++i)
	{
		//builer = 1
		sol = new Solution (this->problem_, 1);
		this->problem_->evaluate (sol);
		sol->get_representation ().str ();
		subpop1->add(sol);

		sol = new Solution (this->problem_, 2);
		this->problem_->evaluate (sol);
		subpop2->add(sol);
		sol->get_representation ().str ();

		sol = new Solution (this->problem_, 3);
		this->problem_->evaluate (sol);
		subpop3->add(sol);
		sol->get_representation ().str ();
	}


	while (maxEvaluations-- > 0) {

		offspring1 = new SolutionSet (populationSize/3);
		offspring2 = new SolutionSet (populationSize/3);
		offspring3 = new SolutionSet (populationSize/3);

		Solution ** individuals = new Solution*[2];

		for (int p = 0; p < 3; p++) {
			for (int i = 0; i < populationSize/3; ++i)
			{

				int pop = rand () % 3;
				int ind = rand () % int(populationSize/3);
				
				if (pop == 1)
					individuals = (Solution **) selection->execute (subpop1);
				else if (pop == 2)
					individuals = (Solution **) selection->execute (subpop2);
				else 
					individuals = (Solution **) selection->execute (subpop3);

				void** objects = new void*[2];
				objects[0] = individuals[0];
				objects[1] = &gs;
				Solution * child = (Solution*) plasmid->execute (objects);

				if (p == 0) {
					offspring1->add (child);
				} else if (p == 1) {
					offspring2->add (child);
				} else {
					offspring3->add (child);
				}

				delete[] objects;
			}
		}

		delete[] individuals;

		// here filtering by nondominance

		for (int p = 0; p < 3; p++) {

			Ranking * ranking;
			SolutionSet * unionSet;
			if (p == 0) {
				unionSet = subpop1->join (offspring1);
				delete offspring1;
				ranking = new Ranking (unionSet);
				this->ns (subpop1, ranking, populationSize, distance);

			} else if (p == 1) {
				unionSet = subpop2->join (offspring2);
				delete offspring2;
				ranking = new Ranking (unionSet);

				this->ns (subpop2, ranking, populationSize, distance);

			} else {
				unionSet = subpop3->join (offspring3);
				delete offspring3;
				ranking = new Ranking (unionSet);
				this->ns (subpop3, ranking, populationSize, distance);
			}

			delete ranking;
			delete unionSet;
		}

	} 

	delete distance;

	SolutionSet * pop = subpop1->join(subpop2);
	SolutionSet * fullPop = pop->join(subpop3);
	Ranking * rnk = new Ranking (fullPop);
	SolutionSet * result = new SolutionSet(rnk->getSubfront(0)->size());

	for (int i=0;i<rnk->getSubfront(0)->size();i++) {
    	result->add(new Solution(rnk->getSubfront(0)->get(i)));
  	}


	return result;

}

void Mota::ns (
	SolutionSet * population, 
	Ranking * ranking, 
	int populationSize,
	Distance * distance)
{

	int remain = populationSize/3;
    int index = 0;
    SolutionSet * front = NULL;
    for (int i=0;i<population->size();i++) {
      delete population->get(i);
    }
    population->clear();

    // Obtain the next front
    front = ranking->getSubfront(index);

    while ((remain > 0) && (remain >= front->size())) {
      //Assign crowding distance to individuals
      distance->crowdingDistanceAssignment(front, problem_->getNumberOfObjectives());

      //Add the individuals of this front
      for (int k = 0; k < front->size(); k++) {
        population->add(new Solution(front->get(k)));
      } // for

      //Decrement remain
      remain = remain - front->size();

      //Obtain the next front
      index++;
      if (remain > 0) {
        front = ranking->getSubfront(index);
      } // if
      
    } // while

    // Remain is less than front(index).size, insert only the best one
    if (remain > 0) {  // front contains individuals to insert
      distance->crowdingDistanceAssignment(front, problem_->getNumberOfObjectives());
      Comparator * c = new CrowdingComparator();
      front->sort(c);
      delete c;
      for (int k = 0; k < remain; k++) {
        population->add(new Solution(front->get(k)));
      } // for

      remain = 0;
    } // if

}