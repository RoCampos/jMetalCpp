#include <iostream>

#include <map>
#include <vector>
#include <link.h>

using namespace std;

int main(int argc, char *argv[])
{

	std::map<rca::Link, std::vector<int>> mapa;	

	rca::Link l (10, 20, 0);
	int GROUPS = 5;

	mapa[l] = std::vector<int>(GROUPS+1);

	auto ref = mapa.at (l);

	ref.at (0) = 1;

	for (auto & e : mapa) {
		cout << e.first << endl;
		auto & ref = e.second;
		for(int i=0; i < GROUPS; i++) {
			cout << i << ":" << ref.at (i) << endl; 
		}
	}

		


	return 0;
}