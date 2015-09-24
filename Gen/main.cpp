#include<iostream>
#include"Genetic.h"
using namespace std;

void main()
{
	GeneticSolver GenSolve;
	GenSolve.ReadData("input.txt");
	vector<Polynom> solves;
	GenSolve.SetMaxIterations(75);
	GenSolve.SetMaxPopulation(150);
	for (int i = 0; i < 100; i++)
	{
		solves.push_back(GenSolve.Solve());
	}
	sort(solves.begin(), solves.end(), [](Polynom &a, Polynom &b) -> bool {return a.StandardDeviation < b.StandardDeviation;});
	cout << "SD: " << solves[0].StandardDeviation << endl;
	cout << "SD: " << solves[1].StandardDeviation << endl;
	cout << "SD: " << solves[2].StandardDeviation << endl;
	for (int c = 0; c < 3; c++)
	{
		for (int i = 0; i < solves[c].Coefficients.size(); i++)
		{
			if (abs(solves[c].Coefficients[i]) > 0.01)
				cout << solves[c].Coefficients[i] << "*x^" << i << " ";
		}
		cout << endl;
	}
	cout << endl;
	system("pause");
}