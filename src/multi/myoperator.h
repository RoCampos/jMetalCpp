#ifndef _MY_OPERATOR__
#define _MY_OPERATOR__

#include <iostream>
#include <map>
#include <vector>

#include <individual.h>
#include <network.h>
#include <path.h>
#include <group.h>
#include <myalgorithm.h>

// extern int OBJ;

struct ShortestPathTree
{
	ShortestPathTree (){}

	void execute (int source, 
		std::vector<int> const & members,
		rca::Network & network, 
		std::vector<rca::Path> & tree) 
	{
		std::vector<std::vector<int>> paths;
		std::vector<int> cpath;
		std::vector<int> bases;
		shortest_path (source, network, bases, cpath, paths);

		for (int m : members) {				
			paths[m].insert (paths[m].begin(), source);
			rca::Path path (std::move (paths[m]));
			tree.push_back (path);
		}
	}
};

struct WidestPathTree
{
	WidestPathTree () {}

	void execute (int source, 
		std::vector<int> const & members,
		rca::Network & network, 
		std::vector<rca::Path> & tree, int tk) 
	{
		std::vector<std::vector<int>> paths;
		std::vector<int> cpath;
		std::vector<int> bases;
		widest_shortest_path (source, network, bases, cpath, paths);

		for (int m : members) {			
			rca::Path path (std::move (paths[m]));			
			tree.push_back (path);
		}

		eval (network, tree, tk);
	}

	void eval (rca::Network & network, 
		std::vector<rca::Path> & tree, 
		int tk) 
	{
		std::vector<rca::Link> curr_tree;

		for (auto & p : tree) {

			for (size_t i = 0; i < p.size ()-1; ++i)
			{
				int v = p.at (i);
				int w = p.at (i+1);
				rca::Link l (v, w, 0);

				//if the edge is not in the curr_tree
				auto res = std::find (curr_tree.begin (), curr_tree.end(), l);
				if (res == curr_tree.end()) {
					curr_tree.push_back (l);
					int band = network.getBand (l.getX(), l.getY()) - tk;
					network.setBand (l.getX(), l.getY(), band);
					network.setBand (l.getY(), l.getX(), band);
				}
			}				
		}
	}

};

struct BestFirstSearch
{
	BestFirstSearch () {}

	void execute (int source, 
		std::vector<int> const & members,
		rca::Network const & network, 
		std::vector<rca::Path> & tree)
	{
		std::vector<std::vector<int>> paths;
		std::vector<int> cpath;
		std::vector<int> bases;
		best_first_search (source, network, bases, cpath, paths);

		for (int m : members) {			
			rca::Path path (std::move (paths[m]));
			tree.push_back (path);
		}
	}
};

void evaluate (Individual & ind, 
	rca::Network & network, 
	std::vector<rca::Group*> const & mgroups, bool widest = false);

void eval (Individual & ind,
	rca::Network & network,
	std::vector<rca::Group*> const &,
	bool widest = false);


bool dominate (
	std::vector<double> & obj1,
	std::vector<double> & obj2);

/**
* Este operador permite reorganizar os caminhos utilizando
* a raiz original da busca. Considerando que 'src' não é 
* a raiz original.
* A saída esperada é o conjunto dtree ordanado pelo src do grupo
* multicast 'i'
*
* @param int source do grupo
* @param int posição da fonte no grupo 'members' unido com 'sk'
* @param DTree estrutura que representa árvores como grupos
* @param vector<rca::Group> grupos multicast
* @param rca::Network rede do problema
*
*/
void reorganize (int src, 
	int pos, 
	int i,	
	DTree & dtree, 
	std::vector<rca::Group*> const & mgroups,
	rca::Network const & network);


#endif // OPERATOR_H
