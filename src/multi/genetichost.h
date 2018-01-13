#ifndef _GENETIC_HOST_
#define _GENETIC_HOST_

#include <path.h>
#include <kspath.h>
#include <network.h>
#include <group.h>
#include <path.h>

//maps a tree
//each slot of the tree is related to one destination
//for each destination there is a set of paths
struct gTree
{
	std::vector<std::vector<rca::Path>> paths;
	void add (std::vector<rca::Path> & paths_) {
		paths.push_back (std::move(paths_));
	}
};


class GeneticHost 
{

public:
	GeneticHost (int K_);
	GeneticHost (std::vector<rca::Group*>& mgroups);
	void execute (rca::Network * network, std::vector<rca::Group*>& mgroups);
	rca::Path getRandomMeme (int tree, int dest);
	rca::Path getBestMeme (int tree, int dest);
	void print ();
	void add (int tree, int dest, rca::Path const&);
	void clear ();


private:
	//number of genetic informations: memes
	int K;
	std::vector<gTree> memes;

};


#endif