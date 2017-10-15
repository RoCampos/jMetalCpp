#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>

#include <path.h>
#include <link.h>
#include <group.h>
#include <network.h>
#include <steiner.h>
#include <steiner_tree_factory.h>
#include <steiner_tree_observer.h>
#include <sttree_local_search.h>
#include <localsearch.h>


typedef rca::EdgeContainer<rca::Comparator, rca::HCell> Container;
typedef rca::sttalgo::SteinerTreeObserver<Container, steiner> Observer;

struct DTree {
	std::vector<rca::Path> paths;
	int cost;
};

struct Individual {

	std::vector<DTree> cromossoma;
	std::vector<double> objectives;	
	std::vector<int> cost;

	std::vector<rca::Link> edges;
	
	Individual (int csize, int _obj = 3) {
		objectives = std::vector<double> (3, 0);
		cromossoma = std::vector<DTree> (csize);
		cost = std::vector<int> (csize);
	}

	double get_objective (int pos) {
		return objectives.at (pos);
	} 

	Individual (){}

	void add_tree (int pos, DTree & tree) {
		cromossoma[pos] = std::move (tree);
	}

	void str () {
		std::cout << "Objective Vector:";
		std::cout << objectives[0] << "\t" << objectives[1] << "\t" << objectives[2] << std::endl;
	}

	void print () {
		for (auto tree : cromossoma) {
			for (auto p : tree.paths) {
				std::cout << p << std::endl;
			}
		}
	}

	void printToEval () {
		int id = 0;
		for (auto tree : cromossoma) {
			std::vector<rca::Link> links;
			for (auto p : tree.paths) {				
				for (size_t i = 0; i < p.size ()-1; ++i) {
					rca::Link e(p[i], p[i+1], 0);
					auto res = std::find (links.begin (), links.end(), e);
					if (res == links.end()) {
						links.push_back (e);
						std::cout<<e.getX()+1<<" - "<<e.getY()+1<<":"<<(id+1)<<";"<<std::endl;
					}	
				}
			}
			id++;
		}	
	}

	void printToEval (std::fstream & output) {
		int id = 0;
		for (auto tree : cromossoma) {
			std::vector<rca::Link> links;
			for (auto p : tree.paths) {				
				for (size_t i = 0; i < p.size ()-1; ++i) {
					rca::Link e(p[i], p[i+1], 0);
					auto res = std::find (links.begin (), links.end(), e);
					if (res == links.end()) {
						links.push_back (e);
						output << e.getX()+1<<" - "<<e.getY()+1<<":"<<(id+1)<<";"<<std::endl;
					}	
				}
			}
			id++;
		}
	}


};

void fix_paths (Individual & novo,
	int SIZET,
	std::vector<int> & prev, 
	std::vector<int> & pathid, 
	int TREE);

void crossover_by_tree (Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo);

void crossover_by_tcost (
	Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo);

void crossover_by_path (
	Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo,
	int NODES);

void crossover_by_hop (
	Individual & ind1, 
	Individual & ind2, 
	Individual & novo,
	int NODES);

void mutation (Individual & ind, 
	rca::Network * network, 
	std::vector<rca::Group> & mgroups);

// ------------------------------------- implementations


#endif // INDIVIDUAL_H