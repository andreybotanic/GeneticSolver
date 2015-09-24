#pragma once
#include<vector>
#include<algorithm>
#include<fstream>
#include<time.h>
using namespace std;

struct Point
{
	float x;
	float y;
};

struct Polynom
{
	vector<float> Coefficients;
	float StandardDeviation;
};

class GeneticSolver
{
private:
	int MaxIterations = 100;
	int MaxPopulation = 100;
	int MinDegree;
	Polynom Best;
	vector<Point> InputData;
	vector<Polynom> Specimens;
	bool DoubleCrossingover = false;
	void GenFirstGeneration(int amount = 10);	// Done
	void calcFitness(Polynom *p);
	void Fintess();								// Done
	void Crossingover(Polynom p1, Polynom p2);	// Done
	void Mutation(Polynom *p);					// Done
public:
	GeneticSolver();
	void ReadData(string fileName);				// Done
	Polynom Solve();							// Done
	void SetMaxIterations(int number);			// Done
	void SetMaxPopulation(int number);			// Done
};