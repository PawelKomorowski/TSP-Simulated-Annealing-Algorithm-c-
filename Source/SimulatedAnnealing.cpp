#include "pch.h"
#include "SimulatedAnnealing.h"


SimulatedAnnealing::SimulatedAnnealing(int n)
{
	srand(time(NULL));
	this->size = n;
	this->path = new int[this->size];
	this->graph = new int*[size];
	for (int i = 0; i < size; i++)
		this->graph[i] = new int[size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			if (i == j)
				graph[i][j] = 0;
			else
				graph[i][j] = rand() % 100 + 1;
		}
	this->weight = -1;
}

SimulatedAnnealing::SimulatedAnnealing(string file_name)
{
	fstream file;
	file.open(file_name, ios::in);
	string pomin;
	int m;
	file >> pomin;
	file >> this->size;
	file >> pomin;
	file >> m;
	getline(file, pomin);
	getline(file, pomin);
	getline(file, pomin);
	for( int i = 0; i < m; i++)
		getline(file, pomin);

	this->path = new int[this->size];
	this->graph = new int*[size];
	for (int i = 0; i < size; i++)
		this->graph[i] = new int[size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			file >> graph[i][j];

	this->weight = -1;
}

SimulatedAnnealing::~SimulatedAnnealing()
{
	for (int i = 0; i < this->size; i++)
		delete[] this->graph[i];
	delete[] this->graph;
	delete[] this->path;
}


void SimulatedAnnealing::print_graph()
{
	cout.width(5);
	cout << internal << " ";
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
	}
	cout << endl;
	for (int i = 0; i < this->size; i++) {
		cout.width(5);
		cout << internal << i;
			for (int j = 0; j < this->size; j++) {
				cout.width(5);
				cout << internal << this->graph[i][j];
			}
		cout << endl;
	}
}

void SimulatedAnnealing::print_path(int tempPath[])
{
	for (int i = 0; i < this->size; i++)
		cout << tempPath[i] << " ";
	cout << endl;
}

int SimulatedAnnealing::get_size()
{
	return this->size;
}

int SimulatedAnnealing::get_weight()
{
	return this->weight;
}

void SimulatedAnnealing::copyPath(int newPath[], int oldPath[])
{
	for (int i = 0; i < size; i++)
		newPath[i] = oldPath[i];
}

void SimulatedAnnealing::setInitTemp(double temp)
{
	this->temperature = temp;
}

void SimulatedAnnealing::setEndTemp(double temp)
{
	this->endTemp = temp;
}

void SimulatedAnnealing::setCooling(double rate)
{
	this->cooling = rate;
}

void SimulatedAnnealing::setIterations(int i)
{
	this->iterations = i;
}

void SimulatedAnnealing::swap(int path[], int i, int j)
{
	int temporary = path[i];
	path[i] = path[j];
	path[j] = temporary;
}

int SimulatedAnnealing::cost(int path[])
{
	int path_cost = 0;
	for (int i = 0; i < this->size; i++) {
		path_cost += graph[path[i]][path[(i + 1) % size]];
	}
	return path_cost;
}

void SimulatedAnnealing::randomizePath(int tempPath[])
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, this->size - 1);
	int a = dis(gen);
	int b = dis(gen);
	int size2 = this->size * 2;
	for (int i = 0; i < size2; i++) {
		a = dis(gen);
		b = dis(gen);
		swap(tempPath, a, b);
	}
}

double SimulatedAnnealing::random()
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}

void SimulatedAnnealing::annaeling() // Symulated annealing algorithm
{
	// Random number generator from 0 to size-1
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, this->size - 1);
	//
	int a, b, oldCost, newCost; // Variables: first index to swap, second index to swap, old path weigth(length), new path weigth
	int *oldPath = new int[this->size]; // Initial path
	for (int i = 0; i < this->size; i++) // Initalize path
		oldPath[i] = i;

	this->randomizePath(oldPath); // Randomize path
	this->copyPath(this->path, oldPath); // Initialize the shortest path(algoritm solution) as initial path
	this->weight = this->cost(this->path); // Initalize the shortest path weight

	int *newPath = new int[this->size]; // New path on which swap will be executed
	this->copyPath(newPath, oldPath); // Copy initial path to new path

	while (this->temperature > this->endTemp) { // Compute algorithm until temperature fall under end temperature(0.00001)
		for (int i = 0; i < this->iterations; i++) { // For each temperature iterate several(determinate) times(100)
			a = dis(gen); // Draw numbers to swap
			do {
				b = dis(gen);
			} while (a == b);
			this->swap(newPath, a, b); // Swap 2 elements

			oldCost = this->cost(oldPath); // Old path weight
			newCost = this->cost(newPath); // New path weight

			if (newCost < oldCost) { // If new weight is lower than old weight then update old path
				this->copyPath(oldPath, newPath);
				if (newCost < this->weight) { // If new weight is lower than the shortest path weight then update shortest path
					this->copyPath(this->path, newPath);
					this->weight = newCost;
				}
			}
			else { // If new cost is bigger on equal then algorithm can take it with probability
				if (this->random() < exp((oldCost - newCost) / this->temperature)) { // If probability is bigger than random number then take worse path to next iteration
					this->copyPath(oldPath, newPath);
				}
				else { // If not then swap back elements i npath
					this->swap(newPath, b, a);
				}
			}
		}
		this->temperature *= this->cooling; // After several inside iteration decrease temperature(cooling)
	}
	delete[] oldPath;
	delete[] newPath;
}