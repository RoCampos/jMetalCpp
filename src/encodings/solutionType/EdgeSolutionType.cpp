
#include <EdgeSolutionType.h>

EdgeSolutionType::EdgeSolutionType (Problem* problem)
: SolutionType (problem) {}

Variable ** EdgeSolutionType::createVariables ()
{


	return (NULL);
}

Individual EdgeSolutionType::get_representation ()
{

	//cast to problem network
	MMRP * mmrp = (MMRP *) problem_;

	//Solution is built here
	int r = rand () % 30;

	int GSIZE = mmrp->numberGroups ();

	//starting MRRP individual representation
	Individual individual = Individual (GSIZE, 3);
	 for (size_t i=0; i < GSIZE; i++) {
		int tk = mmrp->get_group(i)->getTrequest ();
		int src = mmrp->get_group(i)->getSource ();
		std::vector<int> members = mmrp->get_group(i)->getMembers ();
		DTree dtree;		
		members.push_back (src);
		int pos = rand () % members.size ();
		src = members.at (pos);
		members.erase (members.begin ()+pos);
			
		if (r < 10) {
			stp1.execute (src, members, *mmrp->get_network(), dtree.paths, tk);	
		}
		else if (r >= 10 && r < 20) {
			stp2.execute (src, members, *mmrp->get_network(), dtree.paths);	
		}
		else {
			stp3.execute (src, members, *mmrp->get_network(), dtree.paths);	
		}

		reorganize (src, pos, i, dtree, mmrp->get_groups(), *mmrp->get_network());
		individual.add_tree (i, dtree);			
	}
	
	bool widest = true;
	if (widest) {
		std::set<rca::Link> ref = mmrp->get_network() -> getLinks();
		for (auto & l : ref) {
			mmrp->get_network()->setBand (l.getX(), l.getY(), l.getValue());
			mmrp->get_network()->setBand (l.getY(), l.getX(), l.getValue());
		}

	}

	evaluate (individual, 
		*mmrp->get_network(), 
		mmrp->get_groups());

	return individual;

}
