
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
	Operator * transpon;
	Operator * crossover;

	//for crownding distance
	Distance * distance = new Distance();

	//parameters of algorithms
	int populationSize = *(int*) getInputParameter ("populationSize");
	int hostInformationSize = *(int*) getInputParameter("hostInformationSize");
	int elitePopSize = *(int*) getInputParameter ("elitePopSize");
	int maxEvaluations = *(int *) getInputParameter("maxEvaluations");
	rca::Network * copy = (rca::Network *) getInputParameter ("networkCopy");


	//creatig hostInformationSize paths for each pair s,d for all D
	MMRP * mmrp = (MMRP *) problem_;
	GeneticHost gs (hostInformationSize);
	GeneticHost gshop (hostInformationSize, false);
	GeneticHost elitegs(mmrp->get_groups());
	
	gs.execute (mmrp->get_network (), mmrp->get_groups());
	gshop.execute (copy, mmrp->get_groups());

	//solutions sets 
	subpop1 = new SolutionSet ((int)populationSize/3);
	subpop2 = new SolutionSet ((int)populationSize/3);
	subpop3 = new SolutionSet ((int)populationSize/3);

	plasmid 	= operators_["PathPlasmid"];
	selection	= operators_["selection"];
	crossover 	= operators_["diff_cross"];
	float diff_rate = *(float*) getInputParameter("diff_rate");

	Solution * sol;
	for (int i = 0; i < populationSize/3; ++i)
	{
		//builer = 1
		sol = new Solution (this->problem_, 1);
		this->problem_->evaluate (sol);
		// sol->get_representation ().str ();
		subpop1->add(sol);

		sol = NULL;
		sol = new Solution (this->problem_, 2);
		this->problem_->evaluate (sol);
		subpop2->add(sol);
		// sol->get_representation ().str ();

		sol = NULL;
		sol = new Solution (this->problem_, 3);
		this->problem_->evaluate (sol);
		subpop3->add(sol);
		// sol->get_representation ().str ();
	}

	elite = new SolutionSet (elitePopSize);

	while (maxEvaluations-- > 0) {

		offspring1 = new SolutionSet ((populationSize/3)+2);
		offspring2 = new SolutionSet ((populationSize/3)+2);
		offspring3 = new SolutionSet ((populationSize/3)+2);

		Solution ** individuals1 = new Solution*[2];
		Solution ** individuals2 = new Solution*[2];
		void** objects = new void*[2];

		void ** subpop = new void*[3];
		subpop[0] = subpop1;
		subpop[1] = subpop2;
		subpop[2] = subpop3;

		prepareElite (elite, subpop);
		elitegs.clear ();
		eliteGeneticMaterial (elite, elitegs);

		delete[] subpop;

		for (int p = 0; p < 3; p++) {
			for (int i = 0; i < populationSize/3; ++i)
			{

				Solution * child = NULL;

				int pop = PseudoRandom::randInt(0, 2);
				if (pop == 0){
					individuals1 = (Solution **) selection->execute (subpop1);
					individuals2 = (Solution **) selection->execute (subpop1);
				}else if (pop == 1){
					individuals1 = (Solution **) selection->execute (subpop2);
					individuals2 = (Solution **) selection->execute (subpop2);
				}
				else if (pop == 2){
					individuals1 = (Solution **) selection->execute (subpop3);
					individuals2 = (Solution **) selection->execute (subpop3);
				}

				float rnd = PseudoRandom::randDouble();
				if (rnd < diff_rate || diff_rate == 0.0) {
					
					objects[0] = individuals1[0];
					int	pos = PseudoRandom::randInt(0, 1);

					if (pos == 0){
						pos = PseudoRandom::randInt(0, 1);
						if (pos == 0)
							objects[1] = &gs;
						else
							objects[1] = &gshop;
					} else{
						objects[1] = &elitegs;
					} 
						
					child = (Solution*) plasmid->execute (objects);
					
				} //endif if for TA operators
				else {
					//mormal crossover
					Solution **sol  = new Solution*[3];
					objects[0] = individuals1[0];
					sol[0] = individuals1[1];
					sol[1] = individuals2[0];
					sol[2] = individuals2[1];
					objects[1] = sol;

					child = (Solution*) crossover->execute (objects);

					delete sol;
				}

				problem_->evaluate (child);
				if (p == 0) {
					offspring1->add (child);
				} else if (p == 1) {
					offspring2->add (child);
				} else {
					offspring3->add (child);
				}


				// delete[] objects;
			}
		}
		delete[] objects;
		delete[] individuals1;
		delete[] individuals2;

		// here filtering by nondominance

		for (int p = 0; p < 3; p++) {

			Ranking * ranking;
			SolutionSet * unionSet;
			if (p == 0) {
				offspring1->add(new Solution(elite->get(0)));
				offspring1->add(new Solution(elite->get(1)));
				unionSet = subpop1->join (offspring1);
				delete offspring1;
				ranking = new Ranking (unionSet);
				this->ns (subpop1, ranking, populationSize, distance);

			} else if (p == 1) {
				offspring2->add(new Solution(elite->get(2)));
				offspring2->add(new Solution(elite->get(3)));			
				unionSet = subpop2->join (offspring2);
				delete offspring2;
				ranking = new Ranking (unionSet);
				this->ns (subpop2, ranking, populationSize, distance);

			} else {
				offspring3->add(new Solution(elite->get(4)));
				offspring3->add(new Solution(elite->get(5)));
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

	SolutionSet * finalpop = new SolutionSet (populationSize);
	for (int i = 0; i < populationSize/3; ++i)
	{
		finalpop->add (new Solution(subpop1->get(i)));
		finalpop->add (new Solution(subpop2->get(i)));
		finalpop->add (new Solution(subpop3->get(i)));
	}
	
	Ranking * rnk = new Ranking (finalpop);

	SolutionSet * result = new SolutionSet(rnk->getSubfront(0)->size());
	for (int i=0;i<rnk->getSubfront(0)->size();i++) {
    	result->add(new Solution(rnk->getSubfront(0)->get(i)));
  	}

  	delete subpop1;
  	delete subpop2;
  	delete subpop3;
  	delete rnk;
  	delete finalpop;

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

void Mota::prepareElite (SolutionSet * elite, void * objects)
{

	for (int i=0;i<elite->size();i++) {
      delete elite->get(i);
    }
    elite->clear();

	SolutionSet ** subpop = (SolutionSet **) objects;
	Operator * transpon = operators_["Transposon"];
	Operator * selection = operators_["selection"];

	Solution ** eliteInd = (Solution **) selection->execute (subpop[0]);
	
	Solution * nova = (Solution*) transpon->execute (eliteInd[0]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));
	nova = (Solution*) transpon->execute (eliteInd[1]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));

	eliteInd = (Solution **) selection->execute (subpop[1]);
	nova = (Solution*) transpon->execute (eliteInd[0]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));
	nova = (Solution*)transpon->execute (eliteInd[1]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));
	
	eliteInd = (Solution **) selection->execute (subpop[2]);
	nova = (Solution*) transpon->execute (eliteInd[0]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));
	nova = (Solution*) transpon->execute (eliteInd[1]);
	problem_->evaluate (nova);
	elite->add (new Solution (nova));

}

void Mota::eliteGeneticMaterial (
	SolutionSet *elite, 
	GeneticHost & gs)
{


	for (int i = 0; i < elite->size (); ++i)
	{
		Individual & repr = elite->get(i)->get_representation ();
		int CHRMO = repr.cromossoma.size ();
		for (int j = 0; j < CHRMO; ++j)
		{
			DTree & tree = repr.cromossoma.at (j);
			for (int p = 0; p < tree.paths.size (); ++p)
			{
				//tree = j
				//destination = p
				rca::Path & path = tree.paths.at (p);
				gs.add (j, p, path);
			}
		}
	}

}