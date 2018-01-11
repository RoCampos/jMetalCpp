#include <genetichost.h>


GeneticHost::GeneticHost (int K_) : K (K_) {


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

rca::Path GeneticHost::getBestMeme (int tree)
{

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