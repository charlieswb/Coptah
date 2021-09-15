module;

#include <random>

export module Random;

export class Rand_Int {
public:
	Rand_Int(int low, int high) : dist{ low, high } {}
	int operator()() { return dist(re); }
	void seed(int s) { re.seed(s); }
private:
	std::default_random_engine re;
	std::uniform_int_distribution<> dist;
};