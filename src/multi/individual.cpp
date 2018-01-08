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

	//SIZET - médate do vector de caminhos.

	//Função aplicada ao método
	//crossover_by_path
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
	int SIZE = ind1.size();
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
	int SIZE = ind1.size();
	for (int i = 0; i < SIZE; ++i)
	{
		if (ind1.cost.at(i) < ind2.cost.at(i)) {
			novo.cromossoma[i] = (ind1.cromossoma.at (i));
		} else {
			novo.cromossoma[i] = (ind2.cromossoma.at (i));
		}
	}
}

/**
	
	Algoritmo utilizado para fazer um crossover
	de um path.
	Apenas insere um novo caminho em uma solução.

*/
void crossover_by_path (
	Individual const & ind1, 
	Individual const & ind2, 
	Individual & novo,
	int NODES)
{

	//Escolhe árvore para modificar
	int TREE =  rand () % ind1.size();

	//obtém número de caminhos
	int SIZET = ind1.cromossoma[TREE].paths.size ();

	//vetor de anteriores 
	std::vector<int> prev(NODES, -1);

	//id dos caminhos na árvore TREEE
	std::vector<int> pathid(NODES, -1);

	//maior caminho (flag)
	size_t great = 0;

	//Copiando árvores que não serão modificadas
	for (int i = 0; i < TREE; ++i)
	{		
		novo.cromossoma[i] = ind1.cromossoma.at (i);
	}
	for (size_t i = TREE+1; i < ind1.size(); i++) {
		novo.cromossoma[i] = ind1.cromossoma.at (i);	
	}

	//para os caminhos em TREE
	for (int i = 0; i < SIZET/2; ++i)
	{

		//copiando caminho para árvore
		novo.cromossoma[TREE].paths.push_back (ind1.cromossoma[TREE].paths[i]);
		//Pegar maior caminho até SIZET
		//Caminho principal da árvore
		if (novo.cromossoma[TREE].paths.at(i).size () > great) {
			great = i;
		}

		//Mapeamento dos anteriores de cada vértices no
		//CAminho de i=0 até SIZET/2
		rca::Path path = ind1.cromossoma[TREE].paths.at (i);
		for (size_t j=0; j < path.size ()-1; j++) {
			//guarda o anteior de cada vértice
			prev[path[j]] = path[j+1];
			//Guarda o id do caminho
			pathid[path[j]] = i;
		}
	}

	//Passa os
	for (int i = SIZET/2; i < SIZET; ++i)
	{
		//copiando caminho de ind2 para novo indivíduo
		novo.cromossoma[TREE].paths.push_back (ind2.cromossoma[TREE].paths[i]);			
	}

	//Obtendo maior caminho da árvore
	//Caminho principal
	rca::Path path = ind1.cromossoma[TREE].paths.at (great);

	//organizar os caminhos
	fix_paths (novo, SIZET, prev, pathid, TREE);

}

/**
	Crossover utilizado para criar um novo indivíduo
	com os menores caminhos possível
*/
void crossover_by_hop (
	Individual & ind1, 
	Individual & ind2, 
	Individual & novo,
	int NODES)
{

	//vetor de anterioes
	std::vector<int> prev(NODES, -1);

	//tamanho dos indivíduos (número de trees)
	int SIZE = ind1.size();

	for (int i = 0; i < SIZE; ++i)
	{
		//obtém a i-ésima árvore
		DTree & tree1 = ind1.cromossoma.at(i);
		DTree & tree2 = ind2.cromossoma.at(i);

		//árvore vazia.
		DTree & ntree = novo.cromossoma.at (i);

		int PATHS = tree1.paths.size();
		for (int p = 0; p < PATHS; ++p)
		{
			rca::Path p1 = tree1.paths.at (p); 
			rca::Path p2 = tree2.paths.at (p);

			//menor caminho de ind1 e ind2 na i-ésima
			//árvore é guardado no novo indivíduo
			if (p1.size () < p2.size ()) {
				ntree.paths.push_back (p1);
			} else { 
				ntree.paths.push_back (p2);
			}
		}

		//vetor de ateriores
		std::vector<int> prev (NODES, -1);

		//id dos caminhos
		std::vector<int> pathid (NODES, -1);

		//camihos da i-ésima árvore
		std::vector<rca::Path> & paths = ntree.paths;

		//calculo os anteriores do caminho
		//principal
		rca::Path fpath = paths.at (0);
		for (size_t i = 0; i < fpath.size()-1; ++i)
		{
			prev[ fpath[i] ] = fpath[i+1];
			pathid[fpath[i] ] = 0;
		}

		//para todos os caminhos de 'paths'
		for (size_t i = 1; i < paths.size (); ++i)
		{

			//adicionar caminho a árvore através
			//de do caminho 'fpath'
			std::vector<int> newpath;

			//procurar um vértice comum entre
			//'currpath' e 'fpath'
			rca::Path & curpath = paths.at (i);
			bool change = false;
			size_t j = 0;
			for (; j < curpath.size ()-1; ++j)
			{
				//se achar um vértice comum para a busca
				// j é a o caminho (obtido de pathid)
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
				//obtém path que contém vértice comum
				//com 'currpath'
				rca::Path &tpath = paths[pathid[j]];
				//obtém a posição de j no tpath
				size_t rp = tpath.getPosition (j);

				//unir o currpath ao tpath já na árvore
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

	int CROMO = ind1.size();

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

	int SIZE = ind.size();
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

DTree & choose_diff_ind (
	Individual & ind1,
	Individual & ind2,
	Individual & ind3,
	int index, int tree)
{
	if (index == 1) {
		return ind1.cromossoma.at (tree);
	} else if (index == 2) {
		return ind2.cromossoma.at (tree);
	} else if (index == 3) {
		return ind2.cromossoma.at (tree);
	}
}

void diff_cross (
	Individual & ind1,
	Individual & ind2,
	Individual & ind3,
	Individual & novo,
	int NODES)
{
	//A ideia desse procedimento é 
	//criar um novo indivíduo a partir
	//de três indivíduos pais
	//mimetiza a mutação diferencial
	//mas aqui utiliza componentes diferentes

	int INDSIZE = ind1.size ();
	std::vector<int> index = {1, 2, 3};

	//o método pode trocar caminhos ou inserir 
	//uma árvore inteira no novo indivíduo
	for (int i = 0; i < INDSIZE; ++i)
	{
		//ordem dos indivíduos no sorteio
		std::random_shuffle(index.begin(), index.end());
		int method = rand () % 2;
		if (method == 1) {
			//passa árvore
			if (index[0] == 1) {
				novo.cromossoma[i] = (ind1.cromossoma.at (i));
			} else if (index[0] == 2) {
				novo.cromossoma[i] = (ind2.cromossoma.at (i));
			} else if (index[0] == 3) {
				novo.cromossoma[i] = (ind3.cromossoma.at (i));
			}

		} else {
			//passa menor caminho ou menor custo
			// method = rand () % 2;
			method = 0;
			if (method == 0) {
				DTree & tree1 = choose_diff_ind (ind1, 
					ind2, ind3, index[1], i);
				DTree & tree2 = choose_diff_ind (ind1, 
					ind2, ind3, index[2], i);
				//árvore vazia.
				DTree & ntree = novo.cromossoma.at (i);

				int PATHS = tree1.paths.size();
				for (int p = 0; p < PATHS; ++p)
				{
					rca::Path p1 = tree1.paths.at (p); 
					rca::Path p2 = tree2.paths.at (p);

					//menor caminho de ind1 e ind2 na i-ésima
					//árvore é guardado no novo indivíduo
					if (p1.size () < p2.size ()) {
						ntree.paths.push_back (p1);
					} else { 
						ntree.paths.push_back (p2);
					}
				}

				//vetor de ateriores
				std::vector<int> prev (NODES, -1);

				//id dos caminhos
				std::vector<int> pathid (NODES, -1);

				//camihos da i-ésima árvore
				std::vector<rca::Path> & paths = ntree.paths;

				//calculo os anteriores do caminho
				//principal
				rca::Path fpath = paths.at (0);
				for (size_t i = 0; i < fpath.size()-1; ++i)
				{
					prev[ fpath[i] ] = fpath[i+1];
					pathid[fpath[i] ] = 0;
				}

				//para todos os caminhos de 'paths'
				for (size_t i = 1; i < paths.size (); ++i)
				{

					//adicionar caminho a árvore através
					//de do caminho 'fpath'
					std::vector<int> newpath;

					//procurar um vértice comum entre
					//'currpath' e 'fpath'
					rca::Path & curpath = paths.at (i);
					bool change = false;
					size_t j = 0;
					for (; j < curpath.size ()-1; ++j)
					{
						//se achar um vértice comum para a busca
						// j é a o caminho (obtido de pathid)
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
						//obtém path que contém vértice comum
						//com 'currpath'
						rca::Path &tpath = paths[pathid[j]];
						//obtém a posição de j no tpath
						size_t rp = tpath.getPosition (j);

						//unir o currpath ao tpath já na árvore
						for (; rp < tpath.size (); rp++) {
							newpath.push_back (tpath[rp]);
						}
						rca::Path np(newpath);				
						curpath = std::move (np);
					}
				}
			} else {
				//caminho mais barato
			}
		}
	}
}


