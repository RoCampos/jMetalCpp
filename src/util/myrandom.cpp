#include <myrandom.h>

using namespace rca;

template<class Engine, class Dist, typename TypeValue>
myrandom<Engine, Dist, TypeValue>::myrandom() { }

template<class Engine, class Dist, typename TypeValue>
myrandom<Engine, Dist, TypeValue>::myrandom(const myrandom<Engine, Dist, TypeValue> & ref) {
	engine = ref.engine;
	distribution = ref.distribution;
}

template<class Engine, class Dist, typename TypeValue>
myrandom<Engine, Dist, TypeValue>::myrandom (long seed, TypeValue a, TypeValue b) {

	//initializing the engine
	engine = Engine (seed);
		
	//initializing the distribution object
	distribution = Dist (a, b);
}

template<class Engine, class Dist, typename TypeValue>
myrandom<Engine, Dist, TypeValue>::myrandom (const Engine & e, TypeValue a, TypeValue b) {
	engine = e;
	distribution = Dist (a, b);
}

template<class Engine, class Dist, typename TypeValue>
void myrandom<Engine, Dist, TypeValue>::reset_interval (TypeValue a, TypeValue b) {
	distribution = Dist (a,b);
}

template<class Engine, class Dist, typename TypeValue>
TypeValue myrandom<Engine, Dist, TypeValue>::rand () {
	TypeValue w = distribution(engine);
	return w;
}

template<class Engine, class Dist, typename TypeValue>
const Engine& myrandom<Engine, Dist, TypeValue>::get_engine () const {
	return engine;
}

template class rca::myrandom<std::mt19937, std::uniform_int_distribution<int>, int>;
template class rca::myrandom<std::mt19937, std::uniform_real_distribution<double>, double>;