#include <genetichost.h>


GeneticHost::GeneticHost (int K_) : K (K_) {


}


//inicializa o banco de memes
//para cada árvore deverá ser adicionado um meme
//usado para obter informarção da população de elite
GeneticHost::GeneticHost (std::vector<rca::Group*>& mgroups)
{
	int GROUPS = mgroups.size ();
	this->memes = std::vector<gTree> (GROUPS);
	//allocatin memory for the vector
	for (int i = 0; i < GROUPS; ++i)
	{
		std::vector<int> const& members = mgroups[i]->getMembers ();
		int size = members.size ();
		this->memes[i].paths = std::vector<std::vector<rca::Path>> (size);
	}
}

void GeneticHost::execute (
	rca::Network * network, 
	std::vector<rca::Group*>& mgroups) 
{

	int GROUPS = mgroups.size ();
	this->memes = std::vector<gTree> (GROUPS);

	//aqui gera todos os k caminhos
	KShortestPath ks(network);

	for (int g = 0; g < GROUPS; g++) {
		std::vector<int> const& members = mgroups[g]->getMembers ();
		int source = mgroups[g]->getSource ();

		for (int m = 0; m < members.size (); m++) {
			ks.init (source, members[m]);
			std::vector<rca::Path> paths;
			int count = 0;
			while (ks.hasNext () && count < this->K) {
				rca::Path p = ks.next ();
				paths.push_back (p);
				count++;
			}
			//adding a meme to gTree database
			this->memes[g].add (paths);
			ks.clear ();
		}
	}
}

rca::Path GeneticHost::getRandomMeme (int tree, int dest)
{
	//get the correct `tree` - paths for tree 'tree'
	gTree & gtree = this->memes[tree];

	//all paths for destination in pos 'dest'
	std::vector<rca::Path> & paths = gtree.paths.at (dest);

	int rpath = rand () % paths.size ();
	return paths.at (rpath);
}

rca::Path GeneticHost::getBestMeme (int tree, int dest)
{
	auto paths = this->memes[tree].paths.at (dest);
	return paths.at (0);
}

void GeneticHost::add (int tree, int dest, rca::Path const & p)
{
	std::vector<rca::Path> & paths = this->memes[tree].paths.at (dest);
	paths.push_back (p);
}

void GeneticHost::clear () {

	//list of tree
	for (int i = 0; i < this->memes.size (); ++i)
	{
		//each tree has a list of list of paths
		int PATHS = this->memes[i].paths.size ();
		for (int j = 0; j < PATHS; ++j)
		{
			//clearing path list for j-th destinatio node
			//of the i-th tree
			std::vector<rca::Path> & paths = this->memes[i].paths.at (j);
			paths.clear ();
		}
		 
	}

}


void GeneticHost::print () {

	int MEMESIZE = this->memes.size ();
	for (int i = 0; i < MEMESIZE; ++i)
	{
		gTree & tree = this->memes.at (i);
		for (int p = 0; p < tree.paths.size (); ++p)
		{
			std::vector<rca::Path> & paths = tree.paths.at (p);
			for (auto pt : paths) {
				cout << pt << endl;
			}
			cout << endl;
		}
	}
	cout << endl;
	cout << endl;


}