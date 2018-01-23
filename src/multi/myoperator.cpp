#include <myoperator.h>

//int OBJ = 3;

void evaluate (Individual & ind, 
	rca::Network & network, 
	std::vector<rca::Group*> const & mgroups, bool widest) 
{

	int Z = std::numeric_limits<int>::max ();
	int COST = 0;
	unsigned int HOP = 0;

	std::vector<rca::Link> residual;
	
	int croma = 0;

	for (auto const & tree  : ind.cromossoma) {		
		int tk = mgroups.at(croma)->getTrequest ();
		std::vector<rca::Link> curr_tree;
		int curr_cost =  0;
		for (rca::Path p : tree.paths) {//test for hop size - max_path			
			if (p.size () > HOP) {
				HOP = p.size () - 1;
			}
			//iterating over edges of a path of the i-th multicast tree
			for (size_t i = 0; i < p.size ()-1; i++) {
				
				int v = p.at (i);
				int w = p.at (i+1);				
				rca::Link l (v, w, 0);

				//if the edge is not in the curr_tree
				auto res = std::find (curr_tree.begin (), curr_tree.end(), l);

				if (res == curr_tree.end()) {
					//add the cost
					bool isres = false;
					COST += network.getCost (l);
					curr_cost += network.getCost (l);
					//update Z if edge l is not in residual
					auto res = std::find (residual.begin (), residual.end(), l);
					if (res == residual.end()) {
						int band = network.getBand (l.getX(), l.getY()); 
						l.setValue (band-tk);
						isres = true;
						
						if (widest) {
							network.setBand (l.getX(), l.getY(), l.getValue ());
							network.setBand (l.getY(), l.getX(), l.getValue ());
						}

						if (l.getValue () < Z) {
							Z = l.getValue ();
						}

					} else {
						int band = (int)res->getValue () - tk;
						res->setValue (band);
						if (res->getValue () < Z) {
							Z = res->getValue ();							
						}
					}
					curr_tree.push_back (l);
					if (isres) residual.push_back (std::move(l));
				}

			}
		}	

		ind.cost[croma] = curr_cost;
		croma++;
	}

	std::sort (residual.begin(), residual.end());
	ind.edges = std::move (residual);

	ind.objectives[0] = Z * (-1);
	ind.objectives[1] = COST;
	ind.objectives[2] = HOP;

}

void eval (Individual & ind,
	rca::Network & network,
	std::vector<rca::Group*> const & mgroups,
	bool widest)
{

	int GROUPS = mgroups.size ();

	int Z = std::numeric_limits<int>::max();
	int HOP = 0;
	int COST = 0;

	// std::map<rca::Link, std::vector<int>> mapa;
	std::vector<int> uso (network.getNumberEdges (),0);
	std::vector<int> arvore (network.getNumberEdges (), -1);
	
	for (int tree_id = 0; tree_id < ind.size (); tree_id++)
	{
		int tk = mgroups.at (tree_id)->getTrequest ();
		DTree const& tree = ind.cromossoma.at (tree_id);
		//iteration over paths
		auto curr_path = tree.paths.begin ();

		int curr_cost =  0;
		
		for (; curr_path != tree.paths.end (); curr_path++) {
			//test for path
			if (HOP < curr_path->size ()-1)
				HOP = curr_path->size ()-1;

			for (int v = 0; v < (curr_path->size ()-1); v++)
			{
				int v_ = curr_path->at (v);
				int w_ = curr_path->at (v+1);
				int cost = network.getCost (v_,w_);
				rca::Link l(v_, w_, cost);

				int index = network.get_index (l);
				//se a aresta nao foi utilizada nesta árvore
				if (arvore[index] != tree_id) {
					curr_cost += cost;

					//pode ser uma aresta nova ou 
					if (arvore[index] == -1) {
						uso[index] = network.getBand (l.getX(), l.getY()) - tk;
						arvore[index] = tree_id;
					} else {
						//aresta já usada em outra árvore
						uso[index] -= tk;
						arvore[index] = tree_id;
					}

					COST += cost;
					if (uso[index] < Z) {
						Z = uso[index];
					}
				}

			} //end for over vertex of paths
		} //end for over paths of the dtree
		ind.cost.at (tree_id) = curr_cost;
	} //end for over cromossoma

	// cout << "Objective vector(eval): ";
	// cout << Z*-1 << "\t";
	// cout << COST << "\t";
	// cout << HOP << endl;

	ind.objectives[0] = Z * (-1);
	ind.objectives[1] = COST;
	ind.objectives[2] = HOP;

	// for (auto e: mapa)
	// {
	// 	auto ref = e.second;
	// 	int band = network.getBand (e.first.getX(), e.first.getY());
	// 	cout << e.first << "("<< band << ":"<< ref[ref.size ()-1] <<" )"<< endl;
	// 	for(int i=0; i < ref.size ()-1; i++) {
	// 		cout << "\t" << i << ":" << ref[i] << ":" << mgroups.at (i)->getTrequest ();
	// 		cout << endl;
	// 	}
	// }

}

bool dominate (
	std::vector<double> & obj1,
	std::vector<double> & obj2)
{

	int count = 0;
	if (obj1[0] > obj2[0]) {
		count++;
	}

	if (obj1[1] < obj2[1]) {
		count++;
	}

	if (obj1[2] < obj2[2]) {
		count++;
	}

	return (count > 0);
}

void reorganize (int src, 
	int pos, 
	int i,	
	DTree & dtree, 
	std::vector<rca::Group*> const & mgroups,
	rca::Network const & network)
{

	if (src != mgroups[i]->getSource ()) 
	{
		std::vector<int> in = std::vector<int> (network.getNumberNodes (),-1);

		rca::Path path (dtree.paths[dtree.paths.size ()-1]);
		for (size_t i=0; i < path.size ()-1; i++) {
			in[path[i]] = path[i+1];
		}

		for (size_t i = 0; i < dtree.paths.size ()-1; ++i)
		{				
			std::vector<int> newpath;
			rca::Path pp (dtree.paths[i]);
			
			int j = pp.size () -1;
			for (; j >= 0; --j)
			{
				if (in[pp[j]] != -1) {//encontra vértice
					j = pp[j]; //marca vertice em path raiz
					break;
				} else {
					newpath.push_back (pp[j]);
				}
			}
			
			size_t rp = path.getPosition (j);
			for (; rp < path.size (); rp++) {
				newpath.push_back (path[rp]);
			}
			rca::Path np(newpath);
			
			dtree.paths[i] = std::move (np);
		}
		dtree.paths.insert (dtree.paths.begin () + pos, path);
		dtree.paths.erase (dtree.paths.begin() + dtree.paths.size ());
		
	} else {
		for (auto & path : dtree.paths) {
			path.reverse ();
		}
	}

}

