#include"Genetic.h"

void GeneticSolver::GenFirstGeneration(int amount)
{
	Specimens = vector<Polynom>(amount);
	int inc_dec = 1;
	bool inc = false;
	if (InputData[0].y < InputData[1].y)
	{
		inc = true;
	}
	for (int i = 2; i < InputData.size(); i++)
	{
		if (inc)
		{
			if (InputData[i].y < InputData[i - 1].y)
			{
				inc_dec++;
				inc = false;
			}
		}
		else
		{
			if (InputData[i].y > InputData[i - 1].y)
			{
				inc_dec++;
				inc = true;
			}
		}
	}
	inc_dec++;
	for (int i = 0; i < Specimens.size(); i++)
	{
		for (int j = 0; j < inc_dec; j++)
		{
			Specimens[i].Coefficients.push_back((rand() % 100 - rand() % 100) / 10.0);
			while (Specimens[i].Coefficients[j] == 0)
				Specimens[i].Coefficients[j] = (rand() % 100 - rand() % 100) / 10.0;
		}
		calcFitness(&Specimens[i]);
	}
	MinDegree = inc_dec;
}

void GeneticSolver::calcFitness(Polynom *p)
{
	float value;
	float deviation = 0;
	float x;
	for (auto data : InputData)
	{
		value = 0;
		x = 1;
		for (int i = 0; i < p->Coefficients.size(); i++)
		{
			value += p->Coefficients[i] * x;
			x *= data.x;
		}
		deviation += (value - data.y) * (value - data.y);
	}
	p->StandardDeviation = sqrt(deviation / InputData.size());
}

void GeneticSolver::Fintess()
{
	float deviation;
	float value;
	float x;
	for (int j = 0; j < Specimens.size(); j++)
	{
		deviation = 0;
		for (auto data : InputData)
		{
			value = 0;
			x = 1;
			for (int i = 0; i < Specimens[j].Coefficients.size(); i++)
			{
				value += Specimens[j].Coefficients[i] * x;
				x *= data.x;
			}
			deviation += (value - data.y) * (value - data.y);
		}
		Specimens[j].StandardDeviation = sqrt(deviation / InputData.size());
	}
	sort(Specimens.begin(), Specimens.end(), [](Polynom &a, Polynom &b) -> bool {return a.StandardDeviation < b.StandardDeviation;});
	if (Specimens.size() > MaxPopulation)
	{
		Specimens.erase(Specimens.begin() + MaxPopulation, Specimens.end());
	}
}

void GeneticSolver::Crossingover(Polynom p1, Polynom p2)
{
	if (DoubleCrossingover)
	{
	}
	else
	{
		Polynom p_min = p1, p_max = p2;
		if (p_min.Coefficients.size() > p_max.Coefficients.size())
		{
			p_max = p1;
			p_min = p1;
		}
		if (p_min.Coefficients.size() < 2)
			return;
		int position = rand() % (p_min.Coefficients.size() - 1) + 1;
		Polynom child1, child2;
		child1.Coefficients = vector<float>(p_max.Coefficients.size());
		child2.Coefficients = vector<float>(p_min.Coefficients.size());
		for (int i = 0; i < position; i++)
		{
			child1.Coefficients[i] = p_min.Coefficients[i];
			child2.Coefficients[i] = p_max.Coefficients[i];
		}
		for (int i = position; i < child1.Coefficients.size(); i++)
		{
			child1.Coefficients[i] = p_max.Coefficients[i];
			calcFitness(&child1);
		}
		for (int i = position; i < child2.Coefficients.size(); i++)
		{
			child2.Coefficients[i] = p_min.Coefficients[i];
			calcFitness(&child2);
		}
		Specimens.push_back(child1);
		Specimens.push_back(child2);
	}
}

void GeneticSolver::Mutation(Polynom *p)
{
	int type = rand() % (p->Coefficients.size() / 2);
	if (type == 0)
	{
		do
		{
			p->Coefficients.push_back(rand() % 5 - rand() % 5);
		} while (p->Coefficients[p->Coefficients.size() - 1] == 0);
		calcFitness(p);
	}
	else
	{
		int position = 0;
		vector<Polynom> mutated_p(p->Coefficients.size() * 4);
		for (int i = 0; 4 * i < mutated_p.size(); i++)
		{
			mutated_p[4 * i] = *p;
			mutated_p[4 * i + 1] = *p;
			mutated_p[4 * i + 2] = *p;
			mutated_p[4 * i + 3] = *p;
			mutated_p[4 * i].Coefficients[i] += 0.5;
			mutated_p[4 * i + 1].Coefficients[i] -= 0.5;
			mutated_p[4 * i + 2].Coefficients[i] += 0.1;
			mutated_p[4 * i + 3].Coefficients[i] -= 0.1;
			calcFitness(&mutated_p[4 * i]);
			calcFitness(&mutated_p[4 * i + 1]);
			calcFitness(&mutated_p[4 * i + 2]);
			calcFitness(&mutated_p[4 * i + 3]);
		}
		sort(mutated_p.begin(), mutated_p.end(), [](Polynom &a, Polynom &b) -> bool {return a.StandardDeviation < b.StandardDeviation;});
		int pos = 0;
		while (pos < mutated_p.size() && mutated_p[pos].Coefficients[MinDegree] == 0)
			pos++;
		if (pos < mutated_p.size())
			*p = mutated_p[pos];
		else
			*p = mutated_p[0];
	}
}

GeneticSolver::GeneticSolver() {	}

void GeneticSolver::ReadData(string fileName)
{
	ifstream in;
	in.open(fileName);
	Point p;
	while (!in.eof())
	{
		in >> p.x >> p.y;
		InputData.push_back(p);
	}
}

Polynom GeneticSolver::Solve()
{
	int current_gen = 1;
	Specimens.erase(Specimens.begin(), Specimens.end());
	GenFirstGeneration(70);
	Best = Specimens[0];
	float *reverse_coeff;
	float *percent;
	float summ;
	srand(time(0));
	while (Specimens[0].StandardDeviation && current_gen < MaxIterations && Specimens[0].StandardDeviation < 1e10)
	{
		reverse_coeff = new float[Specimens.size()];
		percent = new float[Specimens.size()];
		summ = 0;
		for (int i = 0; i < Specimens.size(); i++)
		{
			reverse_coeff[i] = 1.0 / Specimens[i].StandardDeviation;
			summ += reverse_coeff[i];
		}
		for (int i = 0; i < Specimens.size(); i++)
		{
			reverse_coeff[i] = reverse_coeff[i] * 100.0 / summ;
		}
		percent[0] = reverse_coeff[Specimens.size() - 1];
		for (int i = 1; i < Specimens.size(); i++)
		{
			percent[i] = percent[i - 1] + reverse_coeff[Specimens.size() - i - 1];
		}
		int p1, p2;
		float val;
		for (int i = 0; i < Specimens.size() / 7; i++)	// number of childs
		{
			p1 = 0;
			p2 = 0;
			val = rand() % 1000 / 10.0;
			while (val > percent[p1])
				p1++;
			while (p2 == p1)
			{
				p2 = 0;
				val = rand() % 1000 / 10.0;
				while (val > percent[p2])
					p2++;
			}
			Crossingover(Specimens[p1], Specimens[p2]);
		}
		int mutation_number = rand() % (Specimens.size() / 2);
		for (int i = 0; i < mutation_number; i++)
		{
			Mutation(&Specimens[rand() % (Specimens.size() / 2)]);
		}
		sort(Specimens.begin(), Specimens.end(), [](Polynom &a, Polynom &b) -> bool {return a.StandardDeviation < b.StandardDeviation;});
		if (Specimens[0].StandardDeviation < Best.StandardDeviation)
			Best = Specimens[0];
		if (Specimens.size() > MaxPopulation)
		{
			Specimens.erase(Specimens.begin() + MaxPopulation, Specimens.end());
		}
		Specimens.erase(unique(Specimens.begin(), Specimens.end(), [](Polynom &a, Polynom &b) {return a.Coefficients == b.Coefficients;}), Specimens.end());
		current_gen++;
	}
	Specimens.clear();
	return Best;
}

void GeneticSolver::SetMaxIterations(int number)
{
	MaxIterations = number;
}

void GeneticSolver::SetMaxPopulation(int number)
{
	MaxPopulation = number;
}
