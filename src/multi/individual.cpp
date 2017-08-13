#include <individual.h>

//SIZET diz de onte começar a fixar os paths
//pathid é o o caminho que conecta um vértice a árvore
//TREE indica a árvore que foi modificada
void fix_paths (Individual & novo,
	int SIZET,
	std::vector<int> & prev, 
	std::vector<int> & pathid, 
	int TREE) 
{

	std::vector<rca::Path> & paths = novo.cromossoma[TREE].paths;
	for (int i = SIZET/2; i < SIZET; ++i)
	{
		std::vector<int> newpath;
		rca::Path & curpath = paths.at (i);
		bool change = false;
		size_t j = 0;
		for (; j < curpath.size ()-1; ++j)
		{
			if (prev[ curpath[j] ] != -1) {				
				j = curpath[j];
				change = true;
				break;
			} else {
				newpath.push_back (curpath[j]);
			}
		}

		//pegando caminho que conecta curpath a árvore
		if (change){
			rca::Path &tpath = paths[pathid[j]];
			size_t rp = tpath.getPosition (j);
			for (; rp < tpath.size (); rp++) {
				newpath.push_back (tpath[rp]);
			}
			rca::Path np(newpath);				
			curpath = std::move (np);
		}
	}

}

void crossover_by_tree (Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo) 
{
	int SIZE = ind1.cromossoma.size ();
	int point = rand () % SIZE;	

	for (int i = 0; i <= point; ++i)
	{
		novo.cromossoma[i] = (ind1.cromossoma.at (i));
	}

	for (int i = point+1; i < SIZE; ++i)
	{
		novo.cromossoma[i] = (ind2.cromossoma.at (i));
	}

}

void crossover_by_tcost (
	Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo) 
{
	int SIZE = ind1.cromossoma.size ();
	for (int i = 0; i < SIZE; ++i)
	{
		if (ind1.cost.at(i) < ind2.cost.at(i)) {
			novo.cromossoma[i] = (ind1.cromossoma.at (i));
		} else {
			novo.cromossoma[i] = (ind2.cromossoma.at (i));
		}
	}
}

void crossover_by_path (
	Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo,
	int NODES)
{

	int TREE =  rand () % ind1.cromossoma.size ();
	int SIZET = ind1.cromossoma[TREE].paths.size ();
	std::vector<int> prev(NODES, -1);
	std::vector<int> pathid(NODES, -1);
	size_t great = 0;

	for (int i = 0; i < TREE; ++i)
	{		
		novo.cromossoma[i] = ind1.cromossoma.at (i);
	}
	for (size_t i = TREE+1; i < ind1.cromossoma.size (); i++) {
		novo.cromossoma[i] = ind1.cromossoma.at (i);	
	}

	for (int i = 0; i < SIZET/2; ++i)
	{
		novo.cromossoma[TREE].paths.push_back (ind1.cromossoma[TREE].paths[i]);
		if (novo.cromossoma[TREE].paths.at(i).size () > great) {
			great = i;
		}
		rca::Path path = ind1.cromossoma[TREE].paths.at (i);
		for (size_t j=0; j < path.size ()-1; j++) {
			prev[path[j]] = path[j+1];
			pathid[path[j]] = i;
		}
	}

	for (int i = SIZET/2; i < SIZET; ++i)
	{
		novo.cromossoma[TREE].paths.push_back (ind2.cromossoma[TREE].paths[i]);			
	}

	rca::Path path = ind1.cromossoma[TREE].paths.at (great);

	fix_paths (novo, SIZET, prev, pathid, TREE);

}

void crossover_by_hop (
	Individual & ind1, 
	Individual & ind2, 
	Individual & novo,
	int NODES)
{

	std::vector<int> prev(NODES, -1);

	int SIZE = ind1.cromossoma.size ();

	//para todas as árvores
	for (int i = 0; i < SIZE; ++i)
	{
		DTree & tree1 = ind1.cromossoma.at(i);
		DTree & tree2 = ind2.cromossoma.at(i);
		DTree & ntree = novo.cromossoma.at (i);

		int PATHS = tree1.paths.size();
		for (int p = 0; p < PATHS; ++p)
		{
			rca::Path p1 = tree1.paths.at (p); 
			rca::Path p2 = tree2.paths.at (p);
			if (p1.size () < p2.size ()) {
				ntree.paths.push_back (p1);
			} else { 
				ntree.paths.push_back (p2);
			}
		}
		std::vector<int> prev (NODES, -1);
		std::vector<int> pathid (NODES, -1);
		std::vector<rca::Path> & paths = ntree.paths;

		rca::Path fpath = paths.at (0);
		for (size_t i = 0; i < fpath.size()-1; ++i)
		{
			prev[ fpath[i] ] = fpath[i+1];
			pathid[fpath[i] ] = 0;
		}

		for (size_t i = 1; i < paths.size (); ++i)
		{
			std::vector<int> newpath;
			rca::Path & curpath = paths.at (i);
			bool change = false;
			size_t j = 0;
			for (; j < curpath.size ()-1; ++j)
			{
				if (prev[ curpath[j] ] != -1) {				
					j = curpath[j];
					change = true;
					break;
				} else {
					newpath.push_back (curpath[j]);
				}
			}

			//pegando caminho que conecta curpath a árvore
			if (change){				
				rca::Path &tpath = paths[pathid[j]];
				size_t rp = tpath.getPosition (j);
				for (; rp < tpath.size (); rp++) {
					newpath.push_back (tpath[rp]);
				}
				rca::Path np(newpath);				
				curpath = std::move (np);
			}
		}
	}
}

void crossover_by_edge (
	Individual & ind1, 
	Individual & ind2, 
	Individual & novo) 
{

	double l1 = ind1.edges.at (0).getValue ();
	double l2 = ind2.edges.at (0).getValue ();

	int CROMO = ind1.cromossoma.size ();

	rca::Link l;
	if (l1 < l2) {
		l = ind1.edges.at (0);

		for (int i = 0; i < CROMO; ++i)
		{
			DTree & tree1 = ind1.cromossoma.at(i);
			DTree & tree2 = ind2.cromossoma.at(i);
			DTree & ntree = novo.cromossoma.at (i);

			bool in = false;
			for (size_t p = 0; p < tree1.paths.size (); p++) {
				rca::Path & path = tree1.paths.at (p);
				//se o path contém l, entao adiciona path de ind2
				for (size_t v = 0; v < path.size ()-1; v++) {
					rca::Link ll (path.at (v), path.at (v+1), 0);
					if (l == ll) {
						in = true;
						break;
					}
				}

				ntree.paths.push_back ( 
					(in ? tree2.paths.at(p) : tree1.paths.at (p)) );

			}
		}

		//remove l from ind1 e passes the path from ind2

	} else {
		l = ind2.edges.at (0);		
		//remove l from ind2 e passes the path from ind2

		for (int i = 0; i < CROMO; ++i)
		{
			DTree & tree1 = ind2.cromossoma.at(i);
			DTree & tree2 = ind1.cromossoma.at(i);
			DTree & ntree = novo.cromossoma.at (i);

			bool in = false;
			for (size_t p = 0; p < tree1.paths.size (); p++) {
				rca::Path & path = tree1.paths.at (p);
				//se o path contém l, entao adiciona path de ind2
				for (size_t v = 0; v < path.size ()-1; v++) {
					rca::Link ll (path.at (v), path.at (v+1), 0);
					if (l == ll) {
						in = true;
						break;
					}
				}

				ntree.paths.push_back ( 
					(in ? tree2.paths.at(p) : tree1.paths.at (p)) );

			}
		}
	}

}

void mutation (Individual & ind, 
	rca::Network * network, 
	std::vector<rca::Group> & mgroups)
{

	int SIZE = ind.cromossoma.size ();
	int NODES= network->getNumberNodes ();

	Container cg;
	cg.init_congestion_matrix (NODES);
	cg.init_handle_matrix (NODES);

	std::vector<steiner> trees;

	Observer observer;
	observer.set_container (cg);
	observer.set_network (*network);

	int overcost = 0;

	for (int i = 0; i < SIZE; ++i)
	{
		rca::Group g = mgroups.at (i);		
		std::vector<int> const& members = g.getMembers ();
		int source = g.getSource ();
		int tk = g.getTrequest ();
		
		steiner st (NODES, source,  members);
		observer.set_steiner_tree (st, NODES);

		DTree & tree = ind.cromossoma.at (i);
		for (size_t k = 0; k < tree.paths.size (); ++k)
		{
			rca::Path & path = tree.paths.at (k);
			for (size_t p = 0; p < path.size ()-1; ++p)
			{
				int cost = 
					network->getCost (
						path.at (p), path.at (p+1));
				int band =
					network->getBand (
						path.at (p), path.at (p+1));
				rca::Link l (path.at (p), path.at (p+1), cost);

				observer.add_edge (l.getX(), l.getY(), cost, tk, band);
			}
		}	
		overcost += observer.get_steiner_tree ().get_cost ();
		trees.push_back (observer.get_steiner_tree ());
	}

	//performing cycle local search
	rca::sttalgo::CycleLocalSearch cls(
		*network,
		mgroups,
		cg);	
	int z = cg.top ();
	
	int ocost = overcost;	
	do {
		ocost = overcost;
		cls.apply (trees, overcost, z);	 	
	} while (overcost < ocost);

	for (size_t i = 0; i < trees.size (); ++i)
	{
		rca::Group g = mgroups.at (i);
		std::vector<int> const& members = g.getMembers ();
		int source = g.getSource ();

		DTree & curr_tree = ind.cromossoma.at (i);
		curr_tree.paths.clear ();

		for (size_t m = 0; m < members.size (); ++m)
		{
			std::vector<int> path = 
				trees.at (i).get_path (members.at (m), source);

			rca::Path p (path);
			curr_tree.paths.push_back (std::move(p));
		}
	}

	// evaluate (ind,
	// 	*network,
	// 	mgroups);


}
