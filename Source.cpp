#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <cstdlib>
#include <chrono>

const int PopSize = 100;
const int NbReines = 4;
const int max_iter = 100;
const double pMutate = 0.8;
const double pCrossover = 0.2;

struct Solution {
	std::vector<int> d_colonnes;
	int nbConflits = 0;
};

int alea(int a, int b) {
	return a + rand() % (b - a);
}

bool estConflit(Solution& S, int i, int j) {
	if (i != j)
		return std::abs(i - j) == std::abs(S.d_colonnes[i] - S.d_colonnes[j]);
	else
		return false;
}

int fct_obj(Solution& S) {
	int n = NbReines;
	int compteur = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (estConflit(S, i, j))
				compteur++;
		}
	}
	return compteur / 2;
}

void afficheSolution(const Solution& S) {
	for (const auto& col : S.d_colonnes) {
		std::cout << "  " << col << " ";
	}
	std::cout << "\tNbConflits  = " << S.nbConflits;
	std::cout << std::endl;
}

void afficheSolution2D(const Solution& S) {
	std::cout << std::endl;
	std::string separator = "+---";
	for (int i = 1; i < NbReines; ++i) {
		separator.append("+---");
	}
	separator.append("+\n");
	for (int i = 0; i < S.d_colonnes.size(); ++i) {
		std::cout << separator;
		for (int j = 0; j < S.d_colonnes.size(); ++j) {

			if (i == S.d_colonnes[j])
				std::cout << "| Q ";
			else
				std::cout << "|   ";

		}
		std::cout << "|\t" << i << std::endl;
	}
	std::cout << separator;
}

void affichePopulation(const std::vector<Solution>& pop) {
	for (const auto& s : pop) {
		afficheSolution(s);
	}
}

void afficheSolution(const std::vector<int>& S) {
	for (const auto& col : S) {
		std::cout << "  " << col << " ";
	}
	std::cout << std::endl;
}
void affichePopulation(const std::vector<std::vector<int>>& pop) {
	for (const auto& s : pop) {
		afficheSolution(s);
	}
}

Solution newSolution() {
	Solution S;

	// Initialisation
	for (int i = 0; i < NbReines; ++i) {
		S.d_colonnes.push_back(i);
	}


	// M�lange 
	for (int i = NbReines - 1; i >= 2; --i) {
		int alea1 = alea(0, NbReines);
		std::swap(S.d_colonnes[i], S.d_colonnes[alea1]);
	}

	S.nbConflits = fct_obj(S);

	return S;
}

void evaluate(std::vector<Solution>& pop) {
	for (auto& s : pop) {
		s.nbConflits = fct_obj(s);
	}
}

void initPop(std::vector<Solution>& pop) {
	for (int i = 0; i < PopSize; ++i) {
		pop.push_back(newSolution());
	}

	evaluate(pop);
}

void mutate(Solution& S) {
	int alea1 = alea(0, S.d_colonnes.size());
	int alea2 = alea(0, S.d_colonnes.size());
	while (alea2 == alea1) {
		alea2 = alea(0, S.d_colonnes.size());
	}
	std::swap(S.d_colonnes[alea1], S.d_colonnes[alea2]);
	S.nbConflits = fct_obj(S);
}

void normalize(Solution& S) {
	// Tableau de marque
	std::vector<bool> Contient(S.d_colonnes.size(), false);
	for (int i = 0; i < S.d_colonnes.size(); ++i)
	{
		if (S.d_colonnes[i] < 0 || S.d_colonnes[i] >= S.d_colonnes.size() || Contient[S.d_colonnes[i]]) {
			S.d_colonnes[i] = alea(0, S.d_colonnes.size());
			while (Contient[S.d_colonnes[i]]) {
				S.d_colonnes[i] = alea(0, S.d_colonnes.size());
			}
			Contient[S.d_colonnes[i]] = true;
		}
		else
			Contient[S.d_colonnes[i]] = true;
	}
	S.nbConflits = fct_obj(S);
}

void crossover(Solution& S1, Solution& S2) {
	int k = alea(0, S1.d_colonnes.size());
	Solution s1, s2;
	for (int j = 0; j < k; ++j) {
		s1.d_colonnes.push_back(S1.d_colonnes[j]);
		s2.d_colonnes.push_back(S2.d_colonnes[j]);
	}
	for (int j = k; j < S1.d_colonnes.size(); ++j) {
		s1.d_colonnes.push_back(S2.d_colonnes[j]);
		s2.d_colonnes.push_back(S1.d_colonnes[j]);
	}

	normalize(s1);
	normalize(s2);

	S1 = s1;
	S2 = s2;
}

void triPopulation(std::vector<Solution>& pop) {
	std::vector<int> individu;
	individu.resize(NbReines);
	for (int i = 0; i < PopSize - 1; i++)
	{
		for (int j = i + 1; j < PopSize; j++)
		{
			if (fct_obj(pop[j]) < fct_obj(pop[i]))
			{
				std::swap(pop[j], pop[i]);
			}
		}
	}
}

void main() {
	srand(time(NULL));
	int a, b, Winner, Loser, iter = 0;

	std::vector<Solution> pop;
	initPop(pop);

	bool stop = false;

	while (iter < max_iter && stop == false) {

		triPopulation(pop);

		Winner = alea(0, PopSize);
		Loser = alea(0, PopSize);

		double p = 0;
		for (int i = 0; i < NbReines; ++i) {
			p = 0 + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (1 - 0)));
			if (p < pCrossover) {
				crossover(pop[Loser], pop[Winner]);
			}
			p = 0 + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (1 - 0)));
			if (p < pMutate) {
				mutate(pop[i]);
			}
			triPopulation(pop);

			if (fct_obj(pop[0]) == 0) {
				stop = true;
				afficheSolution(pop[0]);
				afficheSolution2D(pop[0]);
				break;
			}
		}
	}
	if (iter == max_iter) {
		afficheSolution(pop[0]);
		afficheSolution2D(pop[0]);
	}
}
